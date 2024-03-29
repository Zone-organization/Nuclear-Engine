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
       
    }
}