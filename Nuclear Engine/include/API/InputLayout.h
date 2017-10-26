#pragma once
#include <NE_Common.h>
#include <vector>

namespace NuclearRenderer {
	class NRBInputLayout;
}
namespace NuclearEngine {
	namespace API {
		enum class DataType { Float, Float2, Float3, Float4 };

		struct InputLayoutElement
		{
			int index = 0;
			const char* name = "";
			DataType dataType;
			unsigned int stride = 0;
			unsigned int offset = 0;
		};

		class NEAPI InputLayout
		{
		public:
			InputLayout();
			~InputLayout();

			void Push(int index, const char* name, DataType dataType, unsigned int stride, unsigned int offset);

			NuclearRenderer::NRBInputLayout *GetBase();

			//TODO: Implement
			//InputLayoutElement GetBufferElement(int number);

		protected:
			NuclearRenderer::NRBInputLayout *layout;
		};
	}
}	