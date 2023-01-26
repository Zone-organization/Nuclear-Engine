#include "Assets\Shader.h"

namespace Nuclear
{
    namespace Assets
    {
        Shader::Shader()
            : IAsset(AssetType::Shader), mPipeline(this)
        {
            mBuildDesc = Graphics::ShaderBuildDesc();
        }
        Shader::~Shader()
        {
        }

        Uint32 Shader::GetID()
        {
            //TODO: Shaders should have unique id based on their name/source not path
            return std::hash<Core::UUID>{}(mUUID);
        }
        Graphics::ShaderPipeline& Shader::GetShaderPipeline()
        {
            return mPipeline;
        }
        const Graphics::ShaderBuildDesc& Shader::GetShaderBuildDesc() const
        {
            return mBuildDesc;
        }
        const Graphics::ShaderReflection& Shader::GetReflection() const
        {
            return mReflection;
        }
    }
}