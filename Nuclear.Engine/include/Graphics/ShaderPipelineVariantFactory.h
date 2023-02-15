#pragma once
#include <Graphics/ShaderPipelineVariant.h>

namespace Nuclear
{
	namespace Graphics
	{
		class NEAPI ShaderPipelineVariantFactory
		{
		public:
			ShaderPipelineVariantFactory();

			bool CreateForwardVariant(ShaderPipelineVariant& result, ShaderPipeline& pipeline, ShaderPipelineBakingDesc& bakingdesc);
			bool CreateDefferedVariant(ShaderPipelineVariant& result, ShaderPipeline& pipeline, ShaderPipelineBakingDesc& bakingdesc);
		private:
			void SetIfFound(Diligent::IPipelineState* pipeline, Diligent::SHADER_TYPE ShaderType, const Char* Name, Diligent::IDeviceObject* obj);
			void ReflectShaderPipelineVariant(ShaderPipelineVariant& pipeline);
			Uint32 mRenderQueueCounter;
		};

	}
}