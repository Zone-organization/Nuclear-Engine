#pragma once
#include <Core/Path.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <set>
#include <Graphics/ShaderPipelineSwitch.h>
namespace Nuclear
{
	namespace Rendering
	{
		class ShadowPass;
		class ImageBasedLighting;
	}

	namespace Graphics
	{
		enum class ShaderPSOType : Uint8
		{
			Unknown = 0,

			ForwardPipeline,
			DefferedPipeline,
			GBufferPipeline,

			UNKNOWN_MAX = UINT8_MAX
		};

		enum class SupportedRenderingTechnique : Uint8
		{
			Unknown = 0,
			ForwardOnly,
			DefferedOnly,
			ForwardDeffered,

			UNKNOWN_MAX = UINT8_MAX
		};


		enum class ShaderType : Uint8
		{
			Unknown = 0,

			_3DRendering,
			PostFX,

			UNKNOWN_MAX = UINT8_MAX
		};

		struct ShaderObjectCreationDesc
		{
			std::string mName = "";
			Diligent::SHADER_TYPE mType = Diligent::SHADER_TYPE_UNKNOWN;
			std::string mEntrypoint = "main";

			std::string mSource = "";
			Core::Path mPath = "";

			std::set<std::string> mDefines = std::set<std::string>();

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mName, self.mType, self.mEntrypoint, self.mSource, self.mPath, self.mDefines);
			}
		};
		
		struct ShaderPSODesc
		{
			ShaderObjectCreationDesc mVertexShader = ShaderObjectCreationDesc();
			ShaderObjectCreationDesc mPixelShader = ShaderObjectCreationDesc();
			ShaderPSOType mType = ShaderPSOType::Unknown;
			Diligent::GraphicsPipelineDesc GraphicsPipeline = Diligent::GraphicsPipelineDesc();
			std::vector<std::string> mRTsNames = std::vector<std::string>();


			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mVertexShader, self.mPixelShader, self.mType, self.mSource, self.GraphicsPipeline, self.mRTsNames);
			}
		};

		struct ShaderRenderingBakingDesc
		{
			Uint32 mRTWidth = 800;
			Uint32 mRTHeight = 600;

			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			Rendering::ShadowPass* pShadowPass = nullptr;
			Rendering::ImageBasedLighting* pIBLContext = nullptr;

			Diligent::IBuffer* LightsBufferPtr = nullptr;

			bool mRenderSystemHasDefferedPass = false;
			bool mRenderSystemHasShadowPass = false;
		};

		struct ShaderPipelineVariantDesc
		{
			ShaderPSODesc mMainPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mGBufferPSOCreateInfo = ShaderPSODesc();

			bool _isDeffered = false;
			bool _isSkinned = false;
			bool _isShadowed = false;

			std::set<std::string> mDefines = std::set<std::string>();
			Uint32 mHashKey = 0;
		};

		struct ShaderPipelineDesc
		{
			std::string mName = "";

			ShaderPSODesc mForwardPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mDefferedPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mGBufferPSOCreateInfo = ShaderPSODesc();

			ShaderPipelineVariantDesc mInstanceDesc = ShaderPipelineVariantDesc();

			std::vector<ShaderPipelineSwitch> Switches = std::vector<ShaderPipelineSwitch>();

			std::vector<std::string> mDefines = std::vector<std::string>();

			bool isDeffered = false;
			bool isRenderingPipeline = true;
			ShaderRenderingBakingDesc* pBakingDesc;
		};

		struct ShaderBuildDesc
		{
			ShaderPipelineDesc mPipelineDesc = ShaderPipelineDesc();
			ShaderType mType = ShaderType::Unknown;;
			std::vector<std::string> mDefines;
			std::vector<std::string> mExcludedVariants = std::vector<std::string>();

			bool mSupportSkinnedMeshes = false;
			bool mSupportShadows = false;
			SupportedRenderingTechnique mSupportedTechniques = SupportedRenderingTechnique::Unknown;

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mPipelineDesc, self.mType, self.mDefines, self.mExcludedVariants, self.mSupportSkinnedMeshes, self.mSupportShadows);
			}
		};

	}
}