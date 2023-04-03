#include <Scripting/ScriptingModule.h>
#include <Scripting/ScriptingBindings.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <Utilities/Logger.h>
#pragma comment(lib,"mono-2.0-sgen.lib")

namespace Nuclear
{
	namespace Scripting
	{
		bool ScriptingModule::Initialize(const ScriptingModuleDesc& desc)
		{
			std::string lib = desc.mMonoRuntimeDir.GetRealPath() + "/lib";
			std::string etc = desc.mMonoRuntimeDir.GetRealPath() + "/etc";

			mono_set_dirs(lib.c_str(), etc.c_str());

			pRuntimeDomain = mono_jit_init("Scripting");
			
			if (!pRuntimeDomain)
			{
				NUCLEAR_ERROR("[ScriptingModule] Failed to initialize pRuntimeDomain...");
				return false;
			}

			{
				ScriptingAssemblyCreationDesc CoreAssemblyDesc;
				CoreAssemblyDesc.mNamespaceName = "Nuclear";
				CoreAssemblyDesc.mPath = desc.mScriptingCoreAssemblyDir.GetRealPath() + "/Nuclear.ScriptCore.DLL";				
				if (!CreateScriptingAssembly(&mCoreAssembly, CoreAssemblyDesc))
				{
					NUCLEAR_ERROR("[ScriptingModule] Failed to create Nuclear.ScriptCore assembly!");
					return false;
				}
			}

			if(desc.mAutoInitClientAssembly)
			{
				ScriptingAssemblyCreationDesc ClientAssemblyDesc;
				ClientAssemblyDesc.mNamespaceName = desc.mClientNamespace;
				ClientAssemblyDesc.mPath = desc.mClientAssemblyPath.GetRealPath();
				if (!CreateScriptingAssembly(&mClientAssembly, ClientAssemblyDesc))
				{
					NUCLEAR_ERROR("[ScriptingModule] Failed to create client assembly!");
				}
			}

			InitBindings();
			InitCoreAssembly();
			NUCLEAR_INFO("[ScriptingModule] ScriptingModule has been initialized succesfully!");
			return true;
		}
		void ScriptingModule::Shutdown()
		{
			mono_jit_cleanup(pRuntimeDomain);
		}

		bool ScriptingModule::CreateScriptAsset(Assets::Script* script, const std::string& scriptclassname)
		{
			script->mClass = CreateScriptClass(&mClientAssembly, ScriptingClassCreationDesc(scriptclassname));
			script->mConstructor = ScriptCoreClass.GetMethod(".ctor(uint)");
			script->mOnStartMethod = script->mClass.GetMethod("OnStart()");
			script->mOnUpdateMethod = script->mClass.GetMethod("OnUpdate(single)");

			return false;
		}
	
	//	bool ScriptingModule::CreateScript(Assets::Script* script, Scripting::ScriptingAssembly* assembly, const ScriptCreationDesc& desc)
		//{

			//MonoMethodDesc* ptrTickMethodDesc = mono_method_desc_new(".Test:HelloWorld()", false);
			//// = mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

			//MonoMethod* helloworld;
			//if (ptrTickMethodDesc) {
			//	// Get real function
			//	MonoMethod* virtualMethod = mono_method_desc_search_in_class(ptrTickMethodDesc, script->Class);
			//	if (virtualMethod)
			//	{
			//		helloworld = mono_object_get_virtual_method(object.GetMonoObject(), virtualMethod);
			//	}

			//	// Free
			//	mono_method_desc_free(ptrTickMethodDesc);
			//}

			//MonoMethod* helloworld = mono_class_get_method_from_name(script->Class, "HelloWorld", 0);
			//MonoMethod* helloworld = assembly->GetMethod("Nuclear.Test:HelloWorld()");
			//object.CallMethod(helloworld);
		//	return true;
	//	}
		Scripting::ScriptingClass ScriptingModule::CreateScriptClass(Scripting::ScriptingAssembly* assembly, const ScriptingClassCreationDesc& desc)
		{
			Scripting::ScriptingClass result;
			result.mDesc = desc;
			result.pClass = mono_class_from_name(assembly->pImage, desc.mNamespaceName.c_str(), desc.mClassName.c_str());
			
			if (!result.pClass)
			{
				NUCLEAR_ERROR("[ScriptingModule] Failed to get class in C# {0}.{1} !", desc.mNamespaceName, desc.mClassName);
			}
			return result;
		}
		bool ScriptingModule::CreateScriptingAssembly(Scripting::ScriptingAssembly* scriptmodule, const ScriptingAssemblyCreationDesc& desc)
		{
			scriptmodule->pAssembly = mono_domain_assembly_open(pRuntimeDomain, desc.mPath.GetRealPath().c_str());
			scriptmodule->mNamespaceName = desc.mNamespaceName;

			if (!scriptmodule->pAssembly)
			{
				NUCLEAR_ERROR("[ScriptingModule] Failed to open Assembly {0}..." , desc.mPath.GetRealPath());
				return false;
			}

			//Todo: Move?
			scriptmodule->pImage = mono_assembly_get_image(scriptmodule->pAssembly);
			if (!scriptmodule->pImage)
			{
				NUCLEAR_ERROR("[ScriptingModule] Failed to get image from Assembly {0}...", desc.mPath.GetRealPath());
				return false;
			}

			return true;
		}

		std::string ScriptingModule::ToStdString(_MonoString* monostring)
		{
			char* ptr = mono_string_to_utf8(monostring);
			std::string str{ ptr };
			mono_free(ptr);
			return str;
		}
		
		_MonoDomain* ScriptingModule::GetDomain()
		{
			return pRuntimeDomain;
		}

		ScriptingAssembly* ScriptingModule::GetCoreAssembly()
		{
			return &mCoreAssembly;
		}

		ScriptingAssembly* ScriptingModule::GetClientAssembly()
		{
			return &mClientAssembly;
		}

		ScriptingRegistry& ScriptingModule::GetRegistry()
		{
			return mRegistry;
		}
		ScriptingModule::ScriptingModule()
		{
			pRuntimeDomain = nullptr;
		}

		void HelloWorld()
		{
			NUCLEAR_INFO("\n HELLO WORLD \n");
		}
		void ScriptingModule::InitBindings()
		{
			//Utilities::Logger
			mono_add_internal_call("Nuclear.Utilities.Logger::LoggerInfo_Native", &Bindings::Utilities_Logger_Info);
			mono_add_internal_call("Nuclear.Utilities.Logger::LoggerWarn_Native", &Bindings::Utilities_Logger_Warn);
			mono_add_internal_call("Nuclear.Utilities.Logger::LoggerTrace_Native", &Bindings::Utilities_Logger_Trace);
			mono_add_internal_call("Nuclear.Utilities.Logger::LoggerError_Native", &Bindings::Utilities_Logger_Error);
			mono_add_internal_call("Nuclear.Utilities.Logger::LoggerFatal_Native", &Bindings::Utilities_Logger_FatalError);

			//ECS::Entity
			mono_add_internal_call("Nuclear.ECS.Entity::AddComponent_Native", &Bindings::ECS_Entity_AddComponent);
			mono_add_internal_call("Nuclear.ECS.Entity::HasComponent_Native", &Bindings::ECS_Entity_HasComponent);

			//Platform::Input
			mono_add_internal_call("Nuclear.Platform.Input::IsKeyPressed_Native", &Bindings::Platform_Input_IsKeyPressed);

		}

		void ScriptingModule::InitCoreAssembly()
		{
			ScriptingClassCreationDesc desc;
			desc.mNamespaceName = "Nuclear.ECS";
			desc.mClassName = "Entity";
			ScriptCoreClass = CreateScriptClass(&mCoreAssembly, desc);
			mRegistry.RegisterEngineComponents(&mCoreAssembly);
		}
	}
}