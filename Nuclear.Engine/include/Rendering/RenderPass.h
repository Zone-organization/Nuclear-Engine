#pragma once
#include <NE_Common.h>

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

            virtual void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight) {};

        //    virtual bool RequireDefferedPass() = 0;
        };
	}
}