#include "Assets\Shader.h"

namespace Nuclear
{
    namespace Assets
    {
        Shader::Shader()
            : IAsset(AssetType::Shader)
        {
            mBuildDesc = ShaderBuildDesc();
            mPipeline = Graphics::ShaderPipeline(this);
        }
        Shader::~Shader()
        {
        }
    }
}