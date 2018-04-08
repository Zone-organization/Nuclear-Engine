#include <Graphics/API/PixelShader.h>
#include <Graphics\API\Context.h>
#include <Graphics/API/ConstantBuffer.h>
namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			PixelShader::PixelShader()
			{

			}

			PixelShader::~PixelShader()
			{

			}

			void PixelShader::Create(PixelShader* obj, BinaryShaderBlob* Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, Desc)
			}

			void PixelShader::Delete(PixelShader * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
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
