#pragma once
#include <Assets/IAsset.h>
#include <Graphics/ShaderReflection.h>
#include <Graphics\ShaderPipeline.h>
#include <Assets/ImportingDescs.h>

namespace Nuclear
{
    namespace Assets
    {     

        class NEAPI Shader : public IAsset
        {
        public: 
            Shader();
            ~Shader();

            Uint32 GetID();

            Graphics::ShaderPipeline& GetShaderPipeline();
            const Graphics::ShaderBuildDesc& GetShaderBuildDesc() const;
            const Graphics::ShaderReflection& GetReflection() const;

            constexpr static auto serialize(auto& archive, auto& self)
            {
                return archive(self.mBuildDesc, self.mReflection);
            }
        protected:
            friend class Importer;
            friend Serialization::Access;

            //Obtained from text/serialization
            ShaderBuildDesc mBuildDesc;
            Graphics::ShaderReflection mReflection;

            //actual shaders PSOs/SRBs
            Graphics::ShaderPipeline mPipeline;
        };
    }
}