#pragma once
#include <Core/NE_Common.h>

namespace Nuclear
{
	namespace Utilities
	{
		class NEAPI NonCopyable {
		protected:
			NonCopyable() = default;
			~NonCopyable() = default;


			NonCopyable(const NonCopyable&) = delete;
			NonCopyable& operator = (const NonCopyable &) = delete;
		};
	}
} 
