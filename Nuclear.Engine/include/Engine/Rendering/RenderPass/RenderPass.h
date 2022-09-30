#pragma once
#include <Core\NE_Common.h>

namespace Nuclear
{
	namespace Rendering
	{

        class NEAPI RenderPass
        {
        public:
            RenderPass() = default;
            virtual ~RenderPass() = default;

            virtual void Initialize() = 0;
            virtual void Update() = 0;

        };
	}
}