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

			void InputLayout::Push(int index, const char* name, DataType dataType, unsigned int stride, unsigned int offset)
			{
				if (dataType == DataType::Float)
				{
					return layout->Push(index, name, NuclearRenderer::DataType::Float, stride, offset);
				}
				else if (dataType == DataType::Float2)
				{
					return layout->Push(index, name, NuclearRenderer::DataType::Float2, stride, offset);
				}
				else if (dataType == DataType::Float3)
				{
					return layout->Push(index, name, NuclearRenderer::DataType::Float3, stride, offset);
				}
				else if (dataType == DataType::Float4)
				{
					return layout->Push(index, name, NuclearRenderer::DataType::Float4, stride, offset);
				}
				return;
			}

			NuclearRenderer::NRBInputLayout * InputLayout::GetBase()
			{
				return layout;
			}

		/*	InputLayoutElement InputLayout::GetBufferElement(int number)
			{
				NuclearRenderer::NRBInputLayout::ILElement temp;
				temp = layout->GetBufferElement().at(number);
				InputLayoutElement result;
				result.
				return layout->GetBufferElement().at(number);
			}*/
				
		}
}