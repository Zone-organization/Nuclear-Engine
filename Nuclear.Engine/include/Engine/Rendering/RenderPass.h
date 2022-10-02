#pragma once
#include <Core\NE_Common.h>

namespace Nuclear
{
	namespace Rendering
	{
        struct FrameRenderData;
        class NEAPI RenderPass
        {
        public:
            RenderPass() = default;
            virtual ~RenderPass() = default;

            virtual void Update(FrameRenderData* framedata) = 0;

        };
	}
}