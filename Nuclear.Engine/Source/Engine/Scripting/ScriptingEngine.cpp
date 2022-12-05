#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/Scripting/ScriptingBindings.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <Core\Logger.h>
#pragma comment(lib,"mono-2.0-sgen.lib")

namespace Nuclear
{
	namespace Scripting
	{
		bool ScriptingEngine::Initialize(const ScriptingEngineDesc& desc)
		{
			std::string lib = desc.mMonoRuntimeDir.GetRealPath() + "/lib";
			std::string etc = desc.mMonoRuntimeDir.GetRealPath() + "/etc";

			mono_set_dirs(lib.c_str(), etc.c_str());

			pRuntimeDomain = mono_jit_init("Scripting");
			
			if (!pRuntimeDomain)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to initialize pRuntimeDomain...");
				return false;
			}

			{
				ScriptModuleCreationDesc CoreAssemblyDesc;
				CoreAssemblyDesc.mPath = desc.mScriptingCoreAssemblyDir.GetRealPath() + "/Nuclear.ScriptCore.DLL";				
				if (!CreateScriptingAssembly(&mCoreAssembly, CoreAssemblyDesc))
				{
					NUCLEAR_ERROR("[ScriptingEngine] Failed to create Nuclear.ScriptCore assembly!");
					return false;
				}
			}

			if(desc.mAutoInitClientAssembly)
			{
				ScriptModuleCreationDesc ClientAssemblyDesc;
				ClientAssemblyDesc.mPath = desc.mClientAssemblyPath.GetRealPath();
				if (!CreateScriptingAssembly(&mClientAssembly, ClientAssemblyDesc))
				{
					NUCLEAR_ERROR("[ScriptingEngine] Failed to create client assembly!");
				}
			}

			InitBindings();
			InitCoreAssembly();

			NUCLEAR_INFO("[ScriptingEngine] ScriptingEngine has been initalized succesfully!");
			return true;
		}
		void ScriptingEngine::Shutdown()
		{
			mono_jit_cleanup(pRuntimeDomain);
		}

		bool ScriptingEngine::CreateScriptAsset(Assets::Script* script, const std::string& scriptclassname)
		{
			script->mClass = CreateScriptClass(&mClientAssembly, ScriptingClassCreationDesc(scriptclassname));

			script->mOnStartMethod = script->mClass.GetMethod("OnStart()");
			script->mOnUpdateMethod = script->mClass.GetMethod("OnUpdate(single)");

			return false;
		}
	
	//	bool ScriptingEngine::CreateScript(Assets::Script* script, Scripting::ScriptingAssembly* assembly, const ScriptCreationDesc& desc)
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
		Scripting::ScriptingClass ScriptingEngine::CreateScriptClass(Scripting::ScriptingAssembly* assembly, const ScriptingClassCreationDesc& desc)
		{
			Scripting::ScriptingClass result;
			result.mDesc = desc;
			result.pClass = mono_class_from_name(assembly->pImage, desc.mNamespaceName.c_str(), desc.mClassName.c_str());
			
			if (!result.pClass)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to get class in C# {0}.{1} !", desc.mNamespaceName, desc.mClassName);
			}
			return result;
		}
		bool ScriptingEngine::CreateScriptingAssembly(Scripting::ScriptingAssembly* scriptmodule, const ScriptModuleCreationDesc& desc)
		{
			scriptmodule->pAssembly = mono_domain_assembly_open(pRuntimeDomain, desc.mPath.GetRealPath().c_str());


			if (!scriptmodule->pAssembly)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to open Assembly {0}..." , desc.mPath.GetRealPath());
				return false;
			}

			//Todo: Move?
			scriptmodule->pImage = mono_assembly_get_image(scriptmodule->pAssembly);
			if (!scriptmodule->pImage)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to get image from Assembly {0}...", desc.mPath.GetRealPath());
				return false;
			}

			return true;
		}

		std::string ScriptingEngine::ToStdString(_MonoString* monostring)
		{
			char* ptr = mono_string_to_utf8(monostring);
			std::string str{ ptr };
			mono_free(ptr);
			return str;
		}
		
		_MonoDomain* ScriptingEngine::GetDomain()
		{
			return pRuntimeDomain;
		}

		ScriptingAssembly* ScriptingEngine::GetCoreAssembly()
		{
			return &mCoreAssembly;
		}

		ScriptingAssembly* ScriptingEngine::GetClientAssembly()
		{
			return &mClientAssembly;
		}

		ScriptingEngine::ScriptingEngine()
		{
			pRuntimeDomain = nullptr;
		}

		void HelloWorld()
		{
			NUCLEAR_INFO("\n HELLO WORLD \n");
		}
		void ScriptingEngine::InitBindings()
		{
			mono_add_internal_call("Nuclear.Core.Logger::LoggerInfo_Native", &Bindings::Core_Logger_Info);
			mono_add_internal_call("Nuclear.Core.Logger::LoggerWarn_Native", &Bindings::Core_Logger_Warn);
			mono_add_internal_call("Nuclear.Core.Logger::LoggerTrace_Native", &Bindings::Core_Logger_Trace);
			mono_add_internal_call("Nuclear.Core.Logger::LoggerError_Native", &Bindings::Core_Logger_Error);
			mono_add_internal_call("Nuclear.Core.Logger::LoggerFatal_Native", &Bindings::Core_Logger_FatalError);

		//	mono_add_internal_call("Nuclear.ScriptCore::HelloWorld_Native()", &HelloWorld);
		}

		void ScriptingEngine::InitCoreAssembly()
		{
			ScriptingClassCreationDesc desc;
			desc.mNamespaceName = "Nuclear";
			desc.mClassName = "ScriptCore";
			ScriptCoreClass = CreateScriptClass(&mCoreAssembly, desc);
		}

		ScriptingEngine& ScriptingEngine::GetInstance()
		{
			static ScriptingEngine instance;

			return instance;
		}
	}
}