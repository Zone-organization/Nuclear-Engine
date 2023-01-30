#pragma once
#include <NE_Common.h>
#include <Assets/TextureDesc.h>
#include <Diligent/Graphics/GraphicsEngine/interface/InputLayout.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/ShaderTypes.h>
#include <Graphics/ShaderReflection.h>

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

			std::vector<Diligent::LayoutElement> GetBasicVSLayout(bool isDeffered);
			void CreateShader(Diligent::IShader** result, const Graphics::ShaderObjectCreationDesc& desc);
			Diligent::IShaderSourceInputStreamFactory* GetDefaultShaderSourceFactory();

			bool ProcessAndCreatePipeline(
				Diligent::IPipelineState** PipelineState,
				Diligent::GraphicsPipelineStateCreateInfo& Desc,
				const std::vector<Diligent::ShaderResourceVariableDesc>& Resources,
				bool AutoCreateSamplersDesc = true,
				const std::vector<Diligent::ImmutableSamplerDesc>& StaticSamplers = std::vector<Diligent::ImmutableSamplerDesc>());

			std::vector<Diligent::ShaderResourceVariableDesc> ReflectShaderVariables(Diligent::IShader* VShader, Diligent::IShader* PShader);

			void CreateShader(const std::string& source, Diligent::IShader** shader, Diligent::SHADER_TYPE type);

			Diligent::SHADER_RESOURCE_VARIABLE_TYPE ParseNameToGetType(const std::string& name);

		protected:
			Diligent::RefCntAutoPtr<Diligent::IShaderSourceInputStreamFactory> pShaderSourceFactory;

		private:
			GraphicsEngine();
		};
	}
}