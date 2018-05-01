#include <Graphics/API/VertexShader.h>
#include <Graphics\API\Context.h>
#include <Graphics/API/ConstantBuffer.h>
#include <Graphics\GraphicsEngine.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			VertexShader::VertexShader()
			{
				isValid = false;
			}

			VertexShader::~VertexShader()
			{
				isValid = false;
			}

			void VertexShader::Create(VertexShader* obj, BinaryShaderBlob* Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, Desc)

				obj->Reflection = Desc->Reflection;
				obj->isValid = true;
				for (auto func : *GraphicsEngine::GetVertexShaderCreationCallbacks())
				{
					if (!func.isNull())
						func(obj, Desc);
				}
			}

			void VertexShader::Delete(VertexShader * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
					obj->isValid = false;
			}

			void VertexShader::SetConstantBuffer(ConstantBuffer * cbuffer)
			{
				SPECIAL_BASE_API_FUNC_CALL(SetConstantBuffer, cbuffer)
			}

			void VertexShader::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
		}
	}
}