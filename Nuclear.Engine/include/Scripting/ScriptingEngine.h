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
		class ScriptingEngine;
	}
	namespace Scripting
	{
		struct ScriptingAssemblyCreationDesc
		{
			std::string mNamespaceName;
			Core::Path mPath;
		};
		struct ScriptingEngineDesc
		{
			//Directory only
			Core::Path mMonoRuntimeDir;
			Core::Path mScriptingCoreAssemblyDir;

			//Path + filename
			std::string mClientNamespace = "ClientScripts";
			Core::Path mClientAssemblyPath;
			bool mAutoInitClientAssembly = true;
		};
		class NEAPI ScriptingEngine
		{
		public:
			ScriptingEngine(ScriptingEngine const&) = delete;
			void operator=(ScriptingEngine const&) = delete;

			static ScriptingEngine& GetInstance();

			bool Initialize(const ScriptingEngineDesc& desc);

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
			ScriptingEngine();

			void InitBindings();
			void InitCoreAssembly();
			ScriptingClass ScriptCoreClass;

			ScriptingAssembly mCoreAssembly;
			ScriptingAssembly mClientAssembly;

			ScriptingRegistry mRegistry;

			_MonoDomain* pRuntimeDomain;
		};


		//class NEAPI ScriptingEngine
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