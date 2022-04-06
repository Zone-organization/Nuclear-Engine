#pragma once
#include "Engine\Scripting\ScriptingContext.h"

class asIScriptContext;

namespace NuclearEngine
{
	namespace Scripting
	{
		namespace Internal
		{
			class NEAPI AngelScriptingContext : public ScriptingContext
			{
			public:
				bool Prepare(ScriptFunction func) override;

				bool Execute() override;

				bool UnPrepare() override;

				bool Abort() override;

				bool Suspend() override;

				asIScriptContext* _context;
			};
		}
	}
}