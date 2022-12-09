#pragma once
#include <NE_Common.h>
#include <Scripting/MonoDeclarations.h>
#include <string>


namespace Nuclear
{
	namespace Scripting
	{
		//Script Scope "kinda" = MonoAssembly = DLL
		//Engine DLL :  Nuclear.ScriptCore, Client DLL : ClientScripts.dll
		class NEAPI ScriptingAssembly
		{
		public:
			ScriptingAssembly();

			//Namespace should be included "Nuclear.ScriptCore:Load()"
			ScriptFunction GetMethod(const std::string& methodname);
			const std::string& GetNamespaceName() const;

			_MonoAssembly* GetAssembly();
			_MonoImage* GetImage();

		private:
			friend class ScriptingEngine;
			_MonoAssembly* pAssembly;
			_MonoImage* pImage;
			std::string mNamespaceName;
			//	std::unordered_map<Assets::Script*, ScriptData> mImportedScripts;

		};
	}
}
