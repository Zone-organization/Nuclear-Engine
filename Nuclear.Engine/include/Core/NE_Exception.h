#pragma once
#include "NE_Common.h"
#include <exception>
#include <string>

namespace Nuclear {
	namespace Exceptions {

#pragma warning(disable : 4275)
#pragma warning(disable : 4251)

		class NotImplementedException : public std::exception 
		{

		public:

			// Construct with given error message:
			NotImplementedException(const char * error = "Function not Implemented Yet!\n")
			{
				errorMessage = error;
				/*Log.Error(errorMessage);*/
			}

			// Provided for compatibility with std::exception.
			const char * what() const noexcept
			{
				return errorMessage.c_str();
			}

		private:

			std::string errorMessage;
		};

	}
}