#pragma once
#include <Engine/Scripting/ScriptData.h>

struct _MonoDomain;

namespace Nuclear
{
	namespace Systems
	{
		class ScriptingEngine;
	}
	namespace Scripting
	{
		class ScriptingModule;

		enum class ScriptModuleCreationFlags
		{
			ONLY_IF_EXISTS = 0,
			CREATE_IF_NOT_EXISTS = 1,
			ALWAYS_CREATE = 2
		};

		struct ScriptModuleCreationDesc
		{
			std::string mName;
			Core::Path mPath;
			ScriptModuleCreationFlags mFlags;
		};
		struct ScriptingEngineDesc
		{
			Core::Path mMonoRuntimeDir;
		};
		class NEAPI ScriptingEngine
		{
		public:
			ScriptingEngine(ScriptingEngine const&) = delete;
			void operator=(ScriptingEngine const&) = delete;

			static ScriptingEngine& GetInstance();

			bool Initialize(const ScriptingEngineDesc& desc);

			void Shutdown();

			bool CreateScriptingModule(Scripting::ScriptingModule* scriptmodule, const ScriptModuleCreationDesc& desc);


		private:
			ScriptingEngine();

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