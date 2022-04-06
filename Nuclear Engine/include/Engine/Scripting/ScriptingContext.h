#pragma once
#include <Engine/Scripting/ScriptData.h>

namespace NuclearEngine
{
	namespace Scripting
	{
		class NEAPI ScriptingContext
		{
		public:

			virtual bool Prepare(ScriptFunction func) = 0;

			virtual bool Execute() = 0;

			virtual bool UnPrepare() = 0;

			virtual bool Abort() = 0;

			virtual bool Suspend() = 0;
		};
	}
}