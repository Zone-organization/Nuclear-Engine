#include "Engine\Assets\Shader.h"

namespace Nuclear
{
    namespace Assets
    {
        Shader::Shader()
        {
            mBuildDesc = ShaderBuildDesc();
            mPipeline = Graphics::ShaderPipeline(this);
            mType = ShaderType::_3DRendering;
        }
        Shader::~Shader()
        {
        }
    }
}