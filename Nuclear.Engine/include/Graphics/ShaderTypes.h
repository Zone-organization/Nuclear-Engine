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
	}
}