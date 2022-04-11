#pragma once
#include <Engine/Scripting/ScriptData.h>

namespace Nuclear
{
	namespace Scripting
	{
		class NEAPI ScriptingContext
		{
		public:

			virtual int Prepare(ScriptFunction func) = 0;

			virtual int Execute() = 0;

			virtual int UnPrepare() = 0;

			virtual int Abort() = 0;

			virtual int Suspend() = 0;

			virtual int SetObject(ScriptObject obj) = 0;

			virtual int SetArgByte(Uint32 arg, Byte value) = 0;

			virtual int SetArgWord(Uint32 arg, Uint16 value) = 0;

			virtual int SetArgDWord(Uint32 arg, Uint64 value) = 0;

			virtual int SetArgQWord(Uint32 arg, Int64 value) = 0;

			virtual int SetArgFloat(Uint32 arg, float value) = 0;

			virtual int SetArgDouble(Uint32 arg, double value) = 0;

			virtual int SetArgAddress(Uint32 arg, void* addr) = 0;

			virtual int SetArgObject(Uint32 arg, void* obj) = 0;

			virtual int SetArgVarType(Uint32 arg, void* ptr, int typeId) = 0;

			virtual void* GetAddressOfArg(Uint32 arg) = 0;

		};
	}
}