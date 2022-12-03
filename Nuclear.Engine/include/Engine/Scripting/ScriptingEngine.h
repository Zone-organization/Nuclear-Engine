#pragma once
#include <Engine/Scripting/ScriptingAssembly.h>
#include <Engine/Scripting/ScriptingObject.h>
#include <Engine/Assets/Script.h>

struct _MonoDomain;
struct _MonoObject;

namespace Nuclear
{
	namespace Systems
	{
		class ScriptingEngine;
	}
	namespace Scripting
	{
		struct ScriptModuleCreationDesc
		{
			std::string mName;
			Core::Path mPath;
		};

		struct ScriptCreationDesc
		{
			std::string mName;
			std::string mClassName;
			std::string mNamespaceName;
		};
		struct ScriptingEngineDesc
		{
			//Directory only
			Core::Path mMonoRuntimeDir;
			Core::Path mScriptingCoreAssemblyDir;

			//Path + filename
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

			bool CreateScript(Assets::Script* script, Scripting::ScriptingAssembly* assembly,const ScriptCreationDesc& desc);

			bool CreateScriptingAssembly(Scripting::ScriptingAssembly* assembly, const ScriptModuleCreationDesc& desc);

			ScriptingObject CreateScriptObject(Assets::Script* script);

			ScriptingAssembly* GetCoreAssembly();
			ScriptingAssembly* GetClientAssembly();
		private:
			ScriptingEngine();

			ScriptingAssembly mCoreAssembly;
			ScriptingAssembly mClientAssembly;

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