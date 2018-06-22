#include <Engine/Graphics/API/PixelShader.h>
#include <Engine\Graphics\API\Context.h>
#include <Engine/Graphics/API/ConstantBuffer.h>
#include <Engine\Graphics\GraphicsEngine.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			PixelShader::PixelShader()
			{
				isValid = false;
			}

			PixelShader::~PixelShader()
			{
				isValid = false;
			}

			void PixelShader::Create(PixelShader* obj, const BinaryShaderBlob& Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, Desc)

				obj->Reflection = Desc.Reflection;
				obj->isValid = true;
				for (auto func : GraphicsEngine::PixelShaderCreationCallbacks)
				{
					if(!func.isNull())
						func(obj, Desc);
				}
			}

			void PixelShader::Delete(PixelShader * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
				obj->isValid = false;
			}

			void PixelShader::SetConstantBuffer(ConstantBuffer * cbuffer)
			{
				SPECIAL_BASE_API_FUNC_CALL(SetConstantBuffer, cbuffer)
			}

			void PixelShader::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
		}
	}
}
