#pragma once
#include <NE_Common.h>
#include <Graphics/ShaderPipelineVariantFactory.h>
#include <Assets/TextureDesc.h>
#include <Diligent/Graphics/GraphicsEngine/interface/InputLayout.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/ShaderTypes.h>
#include <Graphics/ShaderReflection.h>
#include <unordered_map>

struct SDL_Window;

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
		class Mesh;
	}
	namespace Graphics
	{
		struct GraphicsEngineDesc
		{
			Diligent::RENDER_DEVICE_TYPE mRenderAPI = Diligent::RENDER_DEVICE_TYPE::RENDER_DEVICE_TYPE_D3D11;

			bool InitImGui = true;
			SDL_Window* pWindowHandle;

			Diligent::SwapChainDesc SCDesc;

			//Note: Overrides SCDesc ColorBufferFormat
			bool GammaCorrect = false;
		};

		class NEAPI GraphicsEngine
		{
		public:
			GraphicsEngine(GraphicsEngine const&) = delete;
			void operator=(GraphicsEngine const&) = delete;

			static GraphicsEngine& GetInstance();

			bool Initialize(const GraphicsEngineDesc& desc);
			void Shutdown();

			bool CreateMesh(Assets::Mesh* result);

			bool CreateImage(Assets::Texture* result, Assets::TextureData* desc);

			void CreateImageData(Assets::TextureData* result, const Assets::TextureDesc& desc);

			Assets::TextureUsageType ParseTexUsageFromName(std::string& name);

			bool isGammaCorrect();

			bool ReflectShader(const ShaderBuildDesc& desc, ShaderReflection& out);

			FORCE_INLINE const std::vector<Diligent::LayoutElement>& GetRendering3DInputLayout() const
			{
				return mRendering3D_InputLayout;
			}

			FORCE_INLINE const std::vector<Diligent::LayoutElement>& GetRenderToTextureInputLayout() const
			{
				return mRenderToTextureInputLayout;
			}

			void CreateShader(Diligent::IShader** result, const Graphics::ShaderObjectCreationDesc& desc);
			Diligent::IShaderSourceInputStreamFactory* GetDefaultShaderSourceFactory();

			void InitPSOResources(Diligent::GraphicsPipelineStateCreateInfo& result, PSOResourcesInitInfo& desc);

			void CreateShader(const std::string& source, Diligent::IShader** shader, Diligent::SHADER_TYPE type);

			Diligent::SHADER_RESOURCE_VARIABLE_TYPE ParseNameToGetType(const std::string& name);

			ShaderPipelineVariantFactory& GetDefaultShaderPipelineVariantFactory();

		protected:
			void ReflectShaderResources(Diligent::IShader* Shader, PSOResourcesInitInfo& result);
			Diligent::RefCntAutoPtr<Diligent::IShaderSourceInputStreamFactory> pShaderSourceFactory;
			ShaderPipelineVariantFactory mDefaultVariantFactory;

			std::vector<Diligent::LayoutElement> mRendering3D_InputLayout;
			std::vector<Diligent::LayoutElement> mRenderToTextureInputLayout;

			std::unordered_map<std::string, Diligent::SamplerDesc> mDefaultStaticSamplers;

			Diligent::SamplerDesc SamLinearWrapDesc;
			Diligent::SamplerDesc SamLinearClampDesc;
			Diligent::SamplerDesc SamPointBorderDesc;
			Diligent::SamplerDesc ShadowMapSamplerDesc;
		private:
			GraphicsEngine();
		};
	}
}