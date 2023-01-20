#pragma once
#include <Core/Path.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <set>

namespace Nuclear
{
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