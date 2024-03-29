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

            FORCE_INLINE Graphics::ShaderPipeline& GetShaderPipeline()
            {
                return mPipeline;
            }
            FORCE_INLINE const Graphics::ShaderBuildDesc& GetShaderBuildDesc() const
            {
                return mBuildDesc;
            }
            FORCE_INLINE const Graphics::ShaderReflection& GetReflection() const
            {
                return mReflection;
            }

            constexpr static auto serialize(auto& archive, auto& self)
            {
                return archive(self.mBuildDesc, self.mReflection, self.mPipeline);
            }
        protected:
            friend class Graphics::ShaderPipeline;
            friend class Importer;
            friend Serialization::Access;

            //Obtained from text/serialization
            Graphics::ShaderBuildDesc mBuildDesc;
            Graphics::ShaderReflection mReflection;

            //actual shaders PSOs/SRBs
            Graphics::ShaderPipeline mPipeline;
        };
    }
}