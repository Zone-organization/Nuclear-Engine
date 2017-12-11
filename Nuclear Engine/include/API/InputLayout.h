#pragma once
#include <NE_Common.h>
#include <NuclearCommon\Common_API_Types.h>
#include <vector>

namespace NuclearRenderer {
	class NRBInputLayout;
}
namespace NuclearEngine {
	namespace API {

		class NEAPI InputLayout
		{
		public:
			InputLayout();
			~InputLayout();

			void Push(const char* SemanticName, int SemanticIndex, DataType dataType);

			NuclearRenderer::NRBInputLayout *GetBase();
		protected:
			NuclearRenderer::NRBInputLayout *layout;
		};
	}
}	