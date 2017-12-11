#include "API\InputLayout.h"
#include <NuclearRendererBase\NRBInputLayout.h>

namespace NuclearEngine
{
		namespace API
		{
			InputLayout::InputLayout()
			{
				layout = new NuclearRenderer::NRBInputLayout();
			}

			InputLayout::~InputLayout()
			{
				delete layout;
			}

			void InputLayout::Push(const char* SemanticName, int SemanticIndex, DataType dataType)
			{
				return layout->Push(SemanticName, SemanticIndex, dataType);
			}

			NuclearRenderer::NRBInputLayout * InputLayout::GetBase()
			{
				return layout;
			}
				
		}
}