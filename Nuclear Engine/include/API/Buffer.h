#pragma once
#include <NE_PrivateCommon.h>

namespace NuclearEngine {
	namespace API {

		enum NEAPI BufferUsage {
			Unknown,

			Default = 1,
			Static = 2,
			Dynamic = 3
		};

		enum NEAPI BufferAccess {
			Unknown,
			Read = 1,
			Write = 2,
			ReadAndWrite = 3
		};
	}
}