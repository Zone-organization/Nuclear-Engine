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
		
				if (r == 0)
				{
					Log.Info("[AngelScriptEngine] Initialized Successfully...\n");
					return true;
				}
				return false;

			}
			void AngelScriptEngine::Shutdown()
			{
			}
			bool AngelScriptEngine::CreateScript(Assets::Script* script, const std::string& scriptcode, Scripting::ScriptingModule* scriptmodule)
			{
				// Create a new script module
				asIScriptModule* ptr = static_cast<asIScriptModule*>(scriptmodule->ScriptingModulePtr);
				ptr->AddScriptSection(script->GetStringName().c_str(), scriptcode.c_str());

				// Build the module
				int r = ptr->Build();

				if (r < 0)
				{
					return false;
				}


				asIScriptFunction* func = ptr->GetFunctionByDecl("void main()");
				if (func == 0)
				{
					// The function couldn't be found. Instruct the script writer
					// to include the expected function in the script.
					printf("The script must have the function 'void main()'. Please add it and try again.\n");
					return false;
				}

				// Create our context, prepare it, and then execute
				asIScriptContext* MainContext = ASEngine->CreateContext();
				MainContext->Prepare(func);
				r = MainContext->Execute();
				if (r != asEXECUTION_FINISHED)
				{
					// The execution didn't complete as expected. Determine what happened.
					if (r == asEXECUTION_EXCEPTION)
					{
						// An exception occurred, let the script writer know what happened so it can be corrected.
						printf("An exception '%s' occurred. Please correct the code and try again.\n", MainContext->GetExceptionString());
					}
				}

				return true;
			}
			void AngelScriptEngine::CreateScriptingModule(Scripting::ScriptingModule* scriptmodule, ScriptModuleCreationDesc desc)
			{
				scriptmodule->ScriptingModulePtr = ASEngine->GetModule(desc.mName.c_str(), (asEGMFlags)desc.mFlags);
			}
		}
	}
}