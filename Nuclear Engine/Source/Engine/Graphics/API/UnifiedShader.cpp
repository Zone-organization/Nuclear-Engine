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

			void UnifiedShader::SetConstantBuffer(ConstantBuffer * cbuffer)
			{
				//SPECIAL_BASE_API_FUNC_CALL(SetConstantBuffer, cbuffer)
			}

			void UnifiedShader::Bind()
			{
				BASE_API_FUNC_CALL(Bind)
			}
		}
	}
}