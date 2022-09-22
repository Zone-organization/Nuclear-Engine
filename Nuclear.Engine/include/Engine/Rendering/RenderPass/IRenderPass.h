#pragma once

namespace Nuclear
{
	namespace Rendering
	{

        class IRenderPass
        {
        public:
            IRenderPass() = default;
            virtual ~IRenderPass() = default;

            virtual void Initialize() = 0;
            virtual void Update() = 0;

        };
	}
}