#pragma once
#include <Assets/Common.h>
#include <Graphics/ShaderReflection.h>
#include <Graphics\ShaderPipeline.h>
#include <Assets/AssetLoadingDesc.h>

namespace Nuclear
{
    namespace Assets
    {


        struct ShaderBuildDesc 
        {
            Graphics::ShaderPipelineDesc mPipelineDesc = Graphics::ShaderPipelineDesc();
            ShaderType mType = ShaderType::Unknown;;
            std::vector<std::string> mDefines;

            bool mSupportSkinnedMeshes = false;
            bool mSupportShadows = false;
        };

        class NEAPI Shader : public Asset
        {
        public: 
            Shader();
            ~Shader();

            ShaderBuildDesc mBuildDesc;
            Graphics::ShaderPipeline mPipeline;

        };
    }
}