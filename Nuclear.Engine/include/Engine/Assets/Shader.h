#pragma once
#include <Engine/Assets/Common.h>
#include <Engine/Graphics/ShaderReflection.h>
#include <Engine\Graphics\ShaderPipeline.h>

namespace Nuclear
{
    namespace Assets
    {
        enum class ShaderType
        {
            _3DRendering
        };

        struct ShaderBuildDesc 
        {
            Graphics::ShaderPipelineDesc mPipelineDesc = Graphics::ShaderPipelineDesc();

            bool mSupportForwardRendering = true;
            bool mSupportDefferedRendering = false;
            bool mSupportSkinnedMeshes = false;
            bool mSupportShadows = false;
        };

        class NEAPI Shader : public Asset<Shader>
        {
        public: 
            Shader();
            ~Shader();

            ShaderBuildDesc mBuildDesc;
            Graphics::ShaderPipeline mPipeline;

            ShaderType mType;
        };
    }
}