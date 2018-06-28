#include <Engine/Graphics/API/UnifiedShader.h>
#include <Engine\Graphics\API\Context.h>
#include <Engine/Graphics/API/ConstantBuffer.h>
#include <Engine\Graphics\GraphicsEngine.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			UnifiedShader::UnifiedShader()
			{
				isValid = false;
			}

			UnifiedShader::~UnifiedShader()
			{
				isValid = false;
			}

			void UnifiedShader::Create(UnifiedShader* obj, const BinaryShaderBlob& vshader, const BinaryShaderBlob& pshader)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, vshader, pshader)

				obj->VSReflection = vshader.Reflection;
				obj->PSReflection = pshader.Reflection;

				obj->isValid = true;
		
			}

			void UnifiedShader::Delete(UnifiedShader * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
					obj->isValid = false;
			}

			void UnifiedShader::SetConstantBuffer(ConstantBuffer * cbuffer, const ShaderType& type)
			{
				//SPECIAL_CUSTOM_API_CALL
				if (Graphics::API::Context::isOpenGL3RenderAPI()) 
				{
					GLObject.SetConstantBuffer(&cbuffer->GLObject);
				} 
				else if (Graphics::API::Context::isDirectX11RenderAPI())
				{
					if (type == ShaderType::Vertex)
					{
						DXObject.SetConstantBuffer(&cbuffer->DXObject, VSReflection, type);
					}
					else if (type == ShaderType::Pixel)
					{
						DXObject.SetConstantBuffer(&cbuffer->DXObject, PSReflection, type);
					}
				}

			}

			void UnifiedShader::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
		}
	}
}