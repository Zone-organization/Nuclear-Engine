#pragma once
#ifdef EXPOSE_ANGELSCRIPT_ENGINE

#include "Engine\Scripting\ScriptingContext.h"

class asIScriptContext;

namespace Nuclear
{
	namespace Scripting
	{
		namespace Internal
		{
			class NEAPI AngelScriptingContext : public ScriptingContext
			{
			public:
				int Prepare(ScriptFunction func) override;

				int Execute() override;

				int UnPrepare() override;

				int Abort() override;

				int Suspend() override;

				int SetObject(ScriptObject obj) override;

				int SetArgByte(Uint32 arg, Byte value) override;

				int SetArgWord(Uint32 arg, Uint16 value) override;

				int SetArgDWord(Uint32 arg, Uint64 value) override;

				int SetArgQWord(Uint32 arg, Int64 value) override;

				int SetArgFloat(Uint32 arg, float value) override;

				int SetArgDouble(Uint32 arg, double value) override;

				int SetArgAddress(Uint32 arg, void* addr) override;

				int SetArgObject(Uint32 arg, void* obj) override;

				int SetArgVarType(Uint32 arg, void* ptr, int typeId) override;

				void* GetAddressOfArg(Uint32 arg) override;

				asIScriptContext* _context;
			};
		}
	}
}

#endif