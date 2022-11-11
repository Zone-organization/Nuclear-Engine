#pragma once
#include <Core\NE_Common.h>
#include <Engine/Assets/Common.h>
#include <Engine/Assets/MaterialTypes.h>
#include <Engine\Graphics\RenderTarget.h>

namespace Nuclear
{
    namespace Assets
    {

        enum class ShaderType
        {
            _3DRendering
        };

        struct ShaderDesc 
        {

        };



        //TODO
        class NEAPI Shader : public Asset<Shader>
        {
        public:

            RefCntAutoPtr<IShader> VSShader;
            RefCntAutoPtr<IShader> PSShader;

            ShaderType mType;
        };
    }
}