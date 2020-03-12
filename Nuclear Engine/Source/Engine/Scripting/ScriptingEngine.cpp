#include "Engine\Scripting\ScriptingEngine.h"
#include <AngelScript\angelscript.h>
#include "scriptstdstring/scriptstdstring.h"
#include "scriptbuilder/scriptbuilder.h"

#ifdef NUCLEAR_CONFIG_DEBUG
#pragma comment (lib, "angelscript64d.lib")
#else
#pragma comment (lib, "angelscript64.lib")
#endif

namespace NuclearEngine
{
	namespace Scripting
	{
		static asIScriptEngine* AsEngine = nullptr;

		void MessageCallback(const asSMessageInfo* msg, void* param)
		{
			if (msg->type == asMSGTYPE_WARNING)
			{
				Log.Warning("[ScriptingEngine] {" + std::string(msg->section) +
					"} (" + std::to_string(msg->row)
					+ ", " + std::to_string(msg->col)
					+ ") " + std::string(msg->message) + "\n");
			}
			else if (msg->type == asMSGTYPE_INFORMATION)
			{
				Log.Info("[ScriptingEngine] {" + std::string(msg->section) +
					"} (" + std::to_string(msg->row)
					+ ", " + std::to_string(msg->col)
					+ ") " + std::string(msg->message) + "\n");
			}
			else {
				Log.Error("[ScriptingEngine] {" + std::string(msg->section) +
					"} (" + std::to_string(msg->row)
					+ ", " + std::to_string(msg->col)
					+ ") " + std::string(msg->message) + "\n");
			}
		}

		bool ScriptingEngine::Initialize()
		{
			AsEngine = asCreateScriptEngine();
			if (AsEngine == 0)
			{
				Log.Error("[ScriptingEngine] Failed to create AngelScript Engine!\n");
				return false;
			}

			// Set the message callback to receive information on errors in human readable form.
			AsEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

			// AngelScript doesn't have a built-in string type, as there is no definite standard 
			// string type for C++ applications. Every developer is free to register its own string type.
			// The SDK do however provide a standard add-on for registering a string type, so it's not
			// necessary to implement the registration yourself if you don't want to.
			RegisterStdString(AsEngine);

			// Register the function that we want the scripts to call 
			//r = AsEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert(r >= 0);
			return true;
		}
		void ScriptingEngine::Shutdown()
		{
			AsEngine->ShutDownAndRelease();
		}
	}
}