#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		IPipelineState* RenderingPipeline::GetPipeline()
		{
			return mPipeline.RawPtr();
		}
	}
}