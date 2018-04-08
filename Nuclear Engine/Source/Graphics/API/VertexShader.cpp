#include <Graphics/API/VertexShader.h>
#include <Graphics\API\Context.h>
#include <Graphics/API/ConstantBuffer.h>
namespace NuclearEngine {
	namespace Graphics {
		namespace API {

			VertexShader::VertexShader()
			{

			}

			VertexShader::~VertexShader()
			{

			}

			void VertexShader::Create(VertexShader* obj, BinaryShaderBlob* Desc)
			{
				STATIC_BASE_API_FUNC_CALL_ARGS(Create, Desc)
			}

			void VertexShader::Delete(VertexShader * obj)
			{
				STATIC_BASE_API_FUNC_CALL(Delete)
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