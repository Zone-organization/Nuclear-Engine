#pragma once
#include <Engine/Rendering/RenderPass/IRenderPass.h>
#include <Core/Logger.h>
#include <vector>

namespace Nuclear
{
    namespace Rendering
    {
        class RenderPassManager
        {
        public:
            void Initialize();

            void AddRenderPass(IRenderPass* pass)
            {
                pass->Initialize();
                mRenderPasses.push_back(pass);
            }

            template <typename T>
            T* GetRenderPass()
            {
                static_assert(std::is_base_of<IRenderPass, T>::value, "GetRenderPass<T> class must derive from IRenderPass!");

                for (IRenderPass* pass : mRenderPasses)
                {
                    T* result = dynamic_cast<T*>(pass);
                    if (result)
                    {
                        return result;
                    }
                }
                NUCLEAR_FATAL("[RenderPassManager] GetRenderPass called with undefined RenderPass!");
                return nullptr;
            }

        private:
            std::vector<IRenderPass*> mRenderPasses;
        };
    }
}