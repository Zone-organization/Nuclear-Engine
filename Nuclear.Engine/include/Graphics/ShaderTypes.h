#pragma once
#include <Core/Path.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <set>
#include <Graphics/ShaderPipelineSwitch.h>
#include <Serialization/DiligentSerialization.h>
#include <Utilities/Delegate.h>
namespace Nuclear
{
	namespace Rendering
	{
		class ShadowPass;
		class ImageBasedLighting;
	}

	namespace Graphics
	{
		class ShaderPipelineVariantFactory;

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

			Rendering2D = 1,
			Rendering3D = 2,
			RenderingEffect = 3,
			PostFX = 4,

			UNKNOWN_MAX = UINT8_MAX
		};
		////////////////////////////////////////////////////////////////////////// TODO
		/// Describes the shader stages TODO
		///see Diligent::SHADER_TYPE
		enum class ShaderStage : Uint32
		{
			Unknown = 0x0000, ///< Unknown shader type
			Vertex = 0x0001, ///< Vertex shader
			Pixel = 0x0002, ///< Pixel (fragment) shader
			Geometry = 0x0004, ///< Geometry shader
			Hull = 0x0008, ///< Hull (tessellation control) shader
			Domain = 0x0010, ///< Domain (tessellation evaluation) shader
			Compute = 0x0020, ///< Compute shader
			Amplification = 0x0040, ///< Amplification (task) shader
			Mesh = 0x0080, ///< Mesh shader
		};
		//////////////////////////////////////////////////////////////////////// TODO
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
			bool mValid = false;

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mVertexShader, self.mPixelShader, self.mType, self.GraphicsPipeline, self.mRTsNames, self.mValid);
			}
		};

		struct Rendering3DShaderBakingDesc
		{
			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			Rendering::ShadowPass* pShadowPass = nullptr;
			Rendering::ImageBasedLighting* pIBLContext = nullptr;

			bool mRenderSystemHasDefferedPass = false;
			bool mRenderSystemHasShadowPass = false;
		};

		struct ShaderConstantBufferBinding
		{
			Diligent::SHADER_TYPE mShaderStage = Diligent::SHADER_TYPE_UNKNOWN;
			std::string mName = std::string();
			Diligent::IDeviceObject* pCB = nullptr;
		};

		struct ShaderPipelineBakingDesc
		{
			Uint32 mRTWidth = 800;
			Uint32 mRTHeight = 600;

			Utilities::Delegate<void(ShaderVariantReflection&)> mPostVariantReflectionCallback;
			Rendering3DShaderBakingDesc* pRendering3DBakingDesc = nullptr;
			ShaderPipelineVariantFactory* pVariantsFactory = nullptr;
			std::vector<ShaderConstantBufferBinding> mCBsBindings = std::vector<ShaderConstantBufferBinding>();
			std::set<std::string> mDefines = std::set<std::string>();  //TODO: Currently defines is added to all shader stages and should be separated to specific stages
		};

		struct ShaderPipelineVariantDesc
		{
			ShaderPSODesc mMainPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mGBufferPSOCreateInfo = ShaderPSODesc();

			bool isDeffered = false;
			bool isSkinned = false;
			bool isShadowed = false;

			std::set<std::string> mDefines = std::set<std::string>();
			Uint32 mHashKey = 0;

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mMainPSOCreateInfo, self.mGBufferPSOCreateInfo, self.isDeffered, self.isSkinned, self.isShadowed, self.mDefines, self.mHashKey);
			}
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

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mName, self.mForwardPSOCreateInfo, self.mDefferedPSOCreateInfo, self.mGBufferPSOCreateInfo,
					self.mInstanceDesc, self.Switches, self.mDefines
				);
			}
		};

		struct ShaderBuildDesc
		{
			ShaderPipelineDesc mPipelineDesc = ShaderPipelineDesc();
			ShaderType mType = ShaderType::Unknown;
			std::vector<std::string> mDefines;
			std::vector<std::string> mExcludedVariants = std::vector<std::string>();

			bool mSupportSkinnedMeshes = false;
			bool mSupportShadows = false;
			SupportedRenderingTechnique mSupportedTechniques = SupportedRenderingTechnique::Unknown;

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mPipelineDesc, self.mType, self.mDefines, self.mExcludedVariants, self.mSupportSkinnedMeshes, self.mSupportShadows, self.mSupportedTechniques);
			}
		};

	}
}