#pragma once
#include <Assets/IAsset.h>
#include <Graphics/ShaderReflection.h>
#include <Graphics\ShaderPipeline.h>
#include <Assets/ImportingDescs.h>

namespace Nuclear
{
    namespace Assets
    {


        struct ShaderBuildDesc 
        {
            Graphics::ShaderPipelineDesc mPipelineDesc = Graphics::ShaderPipelineDesc();
            ShaderType mType = ShaderType::Unknown;;
            std::vector<std::string> mDefines;
            std::vector<std::string> mExcludedVariants = std::vector<std::string>();

            bool mSupportSkinnedMeshes = false;
            bool mSupportShadows = false;


            constexpr static auto serialize(auto& archive, auto& self)
            {
               return archive(self.mPipelineDesc, self.mType, self.mDefines, self.mExcludedVariants, self.mSupportSkinnedMeshes, self.mSupportShadows);
            }
        };

        class NEAPI Shader : public IAsset
        {
        public: 
            Shader();
            ~Shader();

            Uint32 GetID();

            ShaderBuildDesc mBuildDesc;
            Graphics::ShaderPipeline mPipeline;

            constexpr static auto serialize(auto& archive, auto& self)
            {
                return archive(self.mBuildDesc);
            }
        };
    }
}