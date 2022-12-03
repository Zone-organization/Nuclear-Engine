#include <Engine/Scripting/ScriptingEngine.h>

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

			NUCLEAR_INFO("[ScriptingEngine] ScriptingEngine has been initalized succesfully!");
			return true;
		}
		void ScriptingEngine::Shutdown()
		{
			mono_jit_cleanup(pRuntimeDomain);
		}
		void HelloWorld()
		{
			NUCLEAR_INFO("\n HELLO WORLD \n");
		}
		bool ScriptingEngine::CreateScript(Assets::Script* script, Scripting::ScriptingAssembly* assembly, const ScriptCreationDesc& desc)
		{
			script->Class = mono_class_from_name(assembly->pImage, desc.mNamespaceName.c_str(), desc.mClassName.c_str());

			if (!script->Class)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed to get class in C# {0}!", script->mClassName);
				return false;
			}
			auto object = CreateScriptObject(script);
			mono_add_internal_call("Nuclear.Test:HelloWorld_Native()", HelloWorld);


			MonoMethod* method = NULL, * m = NULL, * ctor = NULL, * fail = NULL, * mvalues;
			MonoProperty* prop;
			MonoObject* result, * exception;
			MonoString* str;
			char* p;
			void* iter;
			void* args[2];
			int val;

			/* retrieve all the methods we need */
			iter = NULL;
			while ((m = mono_class_get_methods(script->Class, &iter))) {
				if (strcmp(mono_method_get_name(m), "method") == 0) {
					method = m;
				}
				else if (strcmp(mono_method_get_name(m), "Fail") == 0) {
					fail = m;
				}
				else if (strcmp(mono_method_get_name(m), "Values") == 0) {
					mvalues = m;
				}
				else if (strcmp(mono_method_get_name(m), ".ctor") == 0) {
					/* Check it's the ctor that takes two args:
					 * as you see a contrsuctor is a method like any other.
					 */
					MonoMethodSignature* sig = mono_method_signature(m);
					if (mono_signature_get_param_count(sig) == 2) {
						ctor = m;
					}
				}
			}

			MonoMethod* helloworld = assembly->GetMethod("Nuclear.Test:HelloWorld()");
			object.CallMethod(helloworld);
			return true;
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
		ScriptingObject ScriptingEngine::CreateScriptObject(Assets::Script* script)
		{
			ScriptingObject result;

			MonoObject* instance = mono_object_new(pRuntimeDomain, script->Class);

			if (!instance)
			{
				NUCLEAR_ERROR("[ScriptingEngine] Failed instantiate C# class!...");
				return result;
			}

			mono_runtime_object_init(instance);
			result.mHandle = mono_gchandle_new(instance, false);
			return result;
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
		ScriptingEngine& ScriptingEngine::GetInstance()
		{
			static ScriptingEngine instance;

			return instance;
		}
	}
}