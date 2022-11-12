#pragma once
#include <Core\NE_Common.h>
#include <Engine/Assets/Common.h>
#include <Engine/Assets/MaterialTypes.h>
#include <Engine\Graphics\RenderTarget.h>

namespace Nuclear
{
    namespace Assets
    {
        enum class ShaderPipelineType
        {
            StaticMeshesPipeline,
            SkinnedMeshesPipeline
        };

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


            std::unordered_map<ShaderPipelineType, Rendering::ShaderPipeline> mPipelines;
            virtual void ReflectPixelShaderData();


            //This can be filled automatically by ReflectPixelShaderData(), Or fill it manually
            //Note: It is very important to fill it in order for material creation work with the pipeline.
            std::vector<Assets::ShaderTexture> mMaterialTexturesInfo;
            std::vector<Assets::ShaderTexture> mIBLTexturesInfo;

            Assets::ShaderTexture mDirPos_ShadowmapInfo;         //Texture2DArray
            Assets::ShaderTexture mSpot_ShadowmapInfo;         //Texture2DArray
            Assets::ShaderTexture mOmniDir_ShadowmapInfo;        //TextureCubeArray


            ShaderType mType;
        };
    }
}