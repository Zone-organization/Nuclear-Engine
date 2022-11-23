#pragma once
#include <Engine/Assets/Common.h>
#include <Engine/Graphics/ShaderReflection.h>
#include <Engine\Graphics\ShaderPipeline.h>
#include <Engine/Importers/Common.h>

namespace Nuclear
{
    namespace Assets
    {


        struct ShaderBuildDesc 
        {
            Graphics::ShaderPipelineDesc mPipelineDesc = Graphics::ShaderPipelineDesc();
            Importers::ShaderType mType = Importers::ShaderType::Unknown;;
            std::vector<std::string> mDefines;

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

        };
    }
}