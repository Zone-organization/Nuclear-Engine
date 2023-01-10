#pragma once
#include <NE_Common.h>
#include <Assets/ImageData.h>
#include <Diligent/Graphics/GraphicsEngine/interface/InputLayout.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/ShaderTypes.h>

struct SDL_Window;

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
	}
	namespace Graphics
	{
		struct GraphicsEngineDesc
		{
			RENDER_DEVICE_TYPE mRenderAPI = RENDER_DEVICE_TYPE::RENDER_DEVICE_TYPE_D3D11;

			bool InitImGui = true;
			SDL_Window* pWindowHandle;

			SwapChainDesc SCDesc;

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

			bool CreateImage(Assets::Texture* result, Assets::ImageData* desc);

			void CreateImageData(Assets::ImageData* result, const Assets::ImageDesc& desc);

			bool isGammaCorrect();

			std::vector<LayoutElement> GetBasicVSLayout(bool isDeffered);
			void CreateShader(IShader** result, const Graphics::ShaderObjectCreationDesc& desc);
			IShaderSourceInputStreamFactory* GetDefaultShaderSourceFactory();

			bool ProcessAndCreatePipeline(
				IPipelineState** PipelineState,
				GraphicsPipelineStateCreateInfo& Desc,
				const std::vector<ShaderResourceVariableDesc>& Resources,
				bool AutoCreateSamplersDesc = true,
				const std::vector<ImmutableSamplerDesc>& StaticSamplers = std::vector<ImmutableSamplerDesc>());

			std::vector<ShaderResourceVariableDesc> ReflectShaderVariables(IShader* VShader, IShader* PShader);

			void CreateShader(const std::string& source, IShader** shader, SHADER_TYPE type);

			SHADER_RESOURCE_VARIABLE_TYPE ParseNameToGetType(const std::string& name);

		protected:
			RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;

		private:
			GraphicsEngine();
		};
	}
}