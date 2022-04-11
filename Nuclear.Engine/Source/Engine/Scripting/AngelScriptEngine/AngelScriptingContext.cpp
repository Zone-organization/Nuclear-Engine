#define EXPOSE_ANGELSCRIPT_ENGINE
#include "Engine/Scripting/AngelScriptEngine/AngelScriptingContext.h"

#include "..\Source\ThirdParty\angelscript\include\angelscript.h" 

namespace Nuclear
{
	namespace Scripting
	{
		namespace Internal
		{
			int AngelScriptingContext::Prepare(ScriptFunction func)
			{
				return _context->Prepare(static_cast<asIScriptFunction*>(func));
			}
			int AngelScriptingContext::Execute()
			{
				return _context->Execute();

			}
			int AngelScriptingContext::UnPrepare()
			{
				return _context->Unprepare();

			}
			int AngelScriptingContext::Abort()
			{
				return _context->Abort();

			}
			int AngelScriptingContext::Suspend()
			{
				return _context->Suspend();

			}
			int AngelScriptingContext::SetObject(ScriptObject obj)
			{
				return _context->SetObject(obj);
		
			}
			int AngelScriptingContext::SetArgByte(Uint32 arg, Byte value)
			{
				return _context->SetArgByte(arg,value);
			}
			int AngelScriptingContext::SetArgWord(Uint32 arg, Uint16 value)
			{
				return _context->SetArgWord(arg, value);
			}
			int AngelScriptingContext::SetArgDWord(Uint32 arg, Uint64 value)
			{
				return _context->SetArgDWord(arg, value);
			}
			int AngelScriptingContext::SetArgQWord(Uint32 arg, Int64 value)
			{
				return _context->SetArgQWord(arg, value);
			}
			int AngelScriptingContext::SetArgFloat(Uint32 arg, float value)
			{
				return _context->SetArgFloat(arg, value);
			}
			int AngelScriptingContext::SetArgDouble(Uint32 arg, double value)
			{
				return _context->SetArgDouble(arg, value);
			}
			int AngelScriptingContext::SetArgAddress(Uint32 arg, void* addr)
			{
				return _context->SetArgAddress(arg, addr);
			}
			int AngelScriptingContext::SetArgObject(Uint32 arg, void* obj)
			{
				return _context->SetArgObject(arg, obj);
			}
			int AngelScriptingContext::SetArgVarType(Uint32 arg, void* ptr, int typeId)
			{
				return _context->SetArgVarType(arg, ptr, typeId);
			}
			void* AngelScriptingContext::GetAddressOfArg(Uint32 arg)
			{
				return _context->GetAddressOfArg(arg);
			}
		}
	}
}