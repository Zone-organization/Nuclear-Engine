#include "AngelScriptEngine.h"
#include "..\Source\ThirdParty\angelscript\include\angelscript.h" 
#include <assert.h>
namespace NuclearEngine
{
	namespace Scripting
	{
		namespace Internal
		{
			asIScriptEngine* ASEngine = nullptr;

			// Print the script string to the standard output stream
			void print()
			{
				printf("\nhello world\n");
			}

			// Implement a simple message callback function
			void MessageCallback(const asSMessageInfo* msg, void* param)
			{
				std::string msgstr  = std::string(msg->section) 
					+ " (" + std::to_string(msg->row) +", "
					+ std::to_string(msg->col) + ") : "
					+ std::string(msg->message) + "\n";


				if (msg->type == asMSGTYPE_WARNING)
				{
					Log.Warning("[AngelScript] " + msgstr);
				}
				else if (msg->type == asMSGTYPE_INFORMATION)
				{
					Log.Info("[AngelScript] "+  msgstr);
				}
				else {
					Log.Error("[AngelScript] "+ msgstr);
				}
			}

			bool AngelScriptEngine::Initialize()
			{
				int r = 1;
				if (ASEngine == nullptr)
				{
					ASEngine = asCreateScriptEngine();
					r = ASEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert(r >= 0);


					// Register the function that we want the scripts to call 
					r = ASEngine->RegisterGlobalFunction("void print()", asFUNCTION(print), asCALL_CDECL);
				}
		
				if (r < 0)
				{
					return false;
				}


				MainContext._context = ASEngine->CreateContext();

				return true;

			}
			void AngelScriptEngine::Shutdown()
			{
			}
			bool AngelScriptEngine::CreateScript(Assets::Script* script, const std::string& scriptcode, Scripting::ScriptingModule* scriptmodule)
			{
				// Create a new script module
				asIScriptModule* ptr = static_cast<asIScriptModule*>(scriptmodule->ScriptingModulePtr);
				int r = ptr->AddScriptSection(script->GetStringName().c_str(), scriptcode.c_str());

				if (r < 0)
				{
					return false;
				}

				ScriptData data;
				data.mScriptParent = script;
				scriptmodule->mImportedScripts[script] = data;


				return true;
			}
			void AngelScriptEngine::CreateScriptingModule(Scripting::ScriptingModule* scriptmodule, ScriptModuleCreationDesc desc)
			{
				scriptmodule->ScriptingModulePtr = ASEngine->GetModule(desc.mName.c_str(), (asEGMFlags)desc.mFlags);
			}
			bool AngelScriptEngine::BuildScriptingModule(Scripting::ScriptingModule* scriptmodule)
			{		
				// Build the module
				asIScriptModule* ptr = static_cast<asIScriptModule*>(scriptmodule->ScriptingModulePtr);
				int r = ptr->Build();

				if (r < 0)
					return false;
				

				for (auto i : scriptmodule->mImportedScripts)
				{
					std::string str = "void " + i.first->GetStringName() + "::";

					auto dataptr = &scriptmodule->mImportedScripts.at(i.first);
					dataptr->mStartfun = ptr->GetFunctionByDecl((str + "Start()").c_str());

					dataptr->mUpdateFun = ptr->GetFunctionByDecl((str + "Update()").c_str());

				}

				return true;
			}
			ScriptingContext* AngelScriptEngine::GetContext()
			{
				return &MainContext;
			}
		}
	}
}