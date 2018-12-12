#pragma once
#include <Base\NE_Common.h>
#include <LLGL\Input.h>

namespace NuclearEngine 
{
	namespace Input
	{
		namespace Keyboard
		{
			using LLGL::Key;
		}
	}

	namespace Core 
	{
		std::shared_ptr<LLGL::Input> Input;
	}
}