#pragma once
#include <Core/Path.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <set>

namespace Nuclear
{
	namespace Graphics
	{
		enum class ShaderPSOType {
			ForwardPipeline,
			DefferedPipeline,
			GBufferPipeline,
			Unknown
		};

		struct ShaderObjectCreationDesc
		{
			std::string mName = "";
			SHADER_TYPE mType = SHADER_TYPE_UNKNOWN;
			std::string mEntrypoint = "main";

			std::string mSource = "";
			Core::Path mPath = "";

			std::set<std::string> mDefines = std::set<std::string>();
		};

		struct ShaderPSODesc
		{
			ShaderObjectCreationDesc mVertexShader = ShaderObjectCreationDesc();
			ShaderObjectCreationDesc mPixelShader = ShaderObjectCreationDesc();
			ShaderPSOType mType = ShaderPSOType::Unknown;
			GraphicsPipelineDesc GraphicsPipeline = GraphicsPipelineDesc();
			std::vector<std::string> mRTsNames = std::vector<std::string>();
		};
	}
}