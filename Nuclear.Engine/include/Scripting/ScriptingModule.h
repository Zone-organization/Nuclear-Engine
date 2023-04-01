#pragma once
#include <Scripting/ScriptingAssembly.h>
#include <Scripting/ScriptingClass.h>
#include <Scripting/ScriptingObject.h>
#include <Scripting/ScriptingRegistry.h>
#include <Assets/Script.h>
#include <Core/Path.h>

namespace Nuclear
{
	namespace Systems
	{
		class ScriptingModule;
	}
	namespace Scripting
	{
		struct ScriptingAssemblyCreationDesc
		{
			std::string mNamespaceName;
			Core::Path mPath;
		};
		struct ScriptingModuleDesc
		{
			//Directory only
			Core::Path mMonoRuntimeDir;
			Core::Path mScriptingCoreAssemblyDir;

			//Path + filename
			std::string mClientNamespace = "ClientScripts";
			Core::Path mClientAssemblyPath;
			bool mAutoInitClientAssembly = true;
		};
		class NEAPI ScriptingModule
		{
		public:
			ScriptingModule(ScriptingModule const&) = delete;
			void operator=(ScriptingModule const&) = delete;

			static ScriptingModule& GetInstance();

			bool Initialize(const ScriptingModuleDesc& desc);

			void Shutdown();

			bool CreateScriptAsset(Assets::Script* script,const std::string& scriptclassname);

			ScriptingClass CreateScriptClass(Scripting::ScriptingAssembly* assembly, const ScriptingClassCreationDesc& desc);

			bool CreateScriptingAssembly(Scripting::ScriptingAssembly* assembly, const ScriptingAssemblyCreationDesc& desc);

			std::string ToStdString(_MonoString* monostring);

			_MonoDomain* GetDomain();
			ScriptingAssembly* GetCoreAssembly();
			ScriptingAssembly* GetClientAssembly();
			ScriptingRegistry& GetRegistry();
		private:
			ScriptingModule();

			void InitBindings();
			void InitCoreAssembly();
			ScriptingClass ScriptCoreClass;

			ScriptingAssembly mCoreAssembly;
			ScriptingAssembly mClientAssembly;

			ScriptingRegistry mRegistry;

			_MonoDomain* pRuntimeDomain;
		};


		//class NEAPI ScriptingModule
		//{
		//public:
		//	bool Initialize();

		//	void Shutdown();

		//	bool CreateScript(Assets::Script* script, const std::string& scriptcode, Scripting::ScriptingModule* scriptmodule);

		//	void CreateScriptingModule(Scripting::ScriptingModule* scriptmodule, ScriptModuleCreationDesc desc);

		//	bool BuildScriptingModule(Scripting::ScriptingModule* scriptmodule);

		//	ScriptingContext* GetContext();
		//private:
		//	_MonoDomain* pRuntimeDomain;

		//};
	}
}