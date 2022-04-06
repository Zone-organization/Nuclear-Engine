#include "AngelScriptingContext.h"
#include "..\Source\ThirdParty\angelscript\include\angelscript.h" 

namespace NuclearEngine
{
	namespace Scripting
	{
		namespace Internal
		{
			bool AngelScriptingContext::Prepare(ScriptFunction func)
			{
				if (_context->Prepare(static_cast<asIScriptFunction*>(func)) < 0)
				{
					return false;
				}

				return true;
			}
			bool AngelScriptingContext::Execute()
			{
				if (_context->Execute() < 0)
				{
					return false;
				}

				return true;
			}
			bool AngelScriptingContext::UnPrepare()
			{
				if (_context->Unprepare() < 0)
				{
					return false;
				}

				return true;
			}
			bool AngelScriptingContext::Abort()
			{
				if (_context->Abort() < 0)
				{
					return false;
				}

				return true;
			}
			bool AngelScriptingContext::Suspend()
			{
				if (_context->Suspend() < 0)
				{
					return false;
				}

				return true;
			}
		}
	}
}