#pragma once
#include "TestCommon.h"

class DirectX11Tests : public Core::Game
{
};
#if 0
#include <iostream>
class DirectX11Tests : public Core::Game
{
protected:
	Graphics::API::VertexShader VShader;
	Graphics::API::PixelShader PShader;

	Graphics::API::VertexShader ScreenVShader;
	Graphics::API::PixelShader ScreenPShader;

	Graphics::API::VertexBuffer CubeVB;
	Graphics::API::VertexBuffer PlaneVB;
	Graphics::API::VertexBuffer ScreenQuadVB;

	Graphics::API::Sampler LinearSampler;
	Graphics::API::Texture PlaneTex;
	Graphics::API::Texture CubeTex;

	Graphics::API::Sampler ScreenSampler;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D *depthBuffer;

	Components::CameraComponent Camera;

	//Default states
	Graphics::API::CommonStates states;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

	std::string VertexShader = R"(struct VertexInputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer NE_Camera : register(b0)
{
	matrix Model;
	matrix View;
	matrix Projection;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(MeshComponent, input.position);
	output.position = mul(View, output.position);
	output.position = mul(Projection, output.position);

	// Store the input texture for the pixel shader to use.
    output.tex = input.tex;
    return output;
})";

	std::string PixelShader = R"(
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D NE_Tex_Diffuse : register(t0);
SamplerState NE_Tex_Diffuse_Sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return NE_Tex_Diffuse.Sample(NE_Tex_Diffuse_Sampler, input.tex);
}
)";
	std::string ScreenVertexShader = R"(struct VertexInputType
{
    float2 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// CalcSulate the position of the vertex against the world, view, and projection matrices.
	output.position = float4(input.position.x,input.position.y,0.0f,1.0f);
	// Store the input texture for the pixel shader to use.
    output.tex = input.tex;
    return output;
})";

	std::string ScreenPixelShader = R"(
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D ScreenTexture : register(t0);
SamplerState ScreenTexture_Sampler : register(s0);

float4 main(PixelInputType input) : SV_TARGET
{
    return float4(ScreenTexture.Sample(ScreenTexture_Sampler, input.tex).rgb, 1.0f);
}
)";

	Graphics::API::RasterizerState R_State;
public:
	DirectX11Tests()
	{
	}
	void Load()
	{
		Graphics::API::VertexShader::Create(
			&VShader,
			&Graphics::API::CompileShader(VertexShader,
				Graphics::API::ShaderType::Vertex));

		Graphics::API::PixelShader::Create(
			&PShader,
			&Graphics::API::CompileShader(PixelShader,
				Graphics::API::ShaderType::Pixel));

		Graphics::API::VertexShader::Create(
			&ScreenVShader,
			&Graphics::API::CompileShader(ScreenVertexShader,
				Graphics::API::ShaderType::Vertex));

		Graphics::API::PixelShader::Create(
			&ScreenPShader,
			&Graphics::API::CompileShader(ScreenPixelShader,
				Graphics::API::ShaderType::Pixel));


		float cubevertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};

		int windowwidth, windowheight;
		Core::Application::GetSize(windowwidth, windowheight);
		float vertices[20] = {

		};

		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};

		Graphics::API::VertexBufferDesc vDesc;
		vDesc.data = cubevertices;
		vDesc.size = sizeof(cubevertices);
		vDesc.usage = Graphics::API::BufferUsage::Static;
		Graphics::API::VertexBuffer::Create(&CubeVB, vDesc);

		vDesc.data = planeVertices;
		vDesc.size = sizeof(planeVertices);
		Graphics::API::VertexBuffer::Create(&PlaneVB, vDesc);

		vDesc.data = quadVertices;
		vDesc.size = sizeof(quadVertices);
		Graphics::API::VertexBuffer::Create(&ScreenQuadVB, vDesc);

		Graphics::API::InputLayout ShaderIL;
		ShaderIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
		ShaderIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2);

		CubeVB.SetInputLayout(&ShaderIL, &VShader);
		PlaneVB.SetInputLayout(&ShaderIL, &VShader);

		Graphics::API::InputLayout ScreenShaderIL;
		ScreenShaderIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float2);
		ScreenShaderIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2);

		ScreenQuadVB.SetInputLayout(&ScreenShaderIL, &ScreenVShader);


		//Create sampler
		Graphics::API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = Graphics::API::TextureFilter::Point2D;
		Graphics::API::Sampler::Create(&ScreenSampler, Samplerdesc);



		Graphics::API::Texture_Desc ScreenTexDesc;
		ScreenTexDesc.Format = Graphics::API::Format::R8G8B8_UNORM;
		ScreenTexDesc.Type = Graphics::API::TextureType::Texture2D;
		ScreenTexDesc.GenerateMipMaps = false;

		//RT
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = 1024;
		textureDesc.Height = 768;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		result = Graphics::API::DirectX::DX11Context::GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
		if (FAILED(result))
		{
		}

		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		result = Graphics::API::DirectX::DX11Context::GetDevice()->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
		if (FAILED(result))
		{
		}

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		result = Graphics::API::DirectX::DX11Context::GetDevice()->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
		if (FAILED(result))
		{
		}
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBuffer, sizeof(depthBuffer));
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = 1024;
		depthBufferDesc.Height = 768;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = Graphics::API::DirectX::DX11Context::GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &depthBuffer);
		if (FAILED(result))
		{
	
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = Graphics::API::DirectX::DX11Context::GetDevice()->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &m_depthStencilView);
		if (FAILED(result))
		{
		}


		Camera.Initialize(Math::Perspective(Math::radians(45.0f), static_cast<Float32>(Core::Application::GetAspectRatio()), 0.1f, 100.0f));

		VShader.SetConstantBuffer(&Camera.GetCBuffer());

		Graphics::API::Texture_Desc Desc;
		Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
		Desc.Type = Graphics::API::TextureType::Texture2D;


		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &PlaneTex, Desc);
		Managers::AssetManager::CreateTextureFromFile("Assets/Common/Textures/crate_diffuse.png", &CubeTex, Desc);

		//Create sampler
		Samplerdesc.Filter = Graphics::API::TextureFilter::Trilinear;
		Graphics::API::Sampler::Create(&LinearSampler, Samplerdesc);

		Graphics::API::RasterizerStateDesc rasterizerdesc;
		rasterizerdesc.FillMode = Graphics::API::FillMode::Wireframe;

		Graphics::API::RasterizerState::Create(&R_State, rasterizerdesc);

		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
		states.EnabledDepth_DisabledStencil.Bind();

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	void OnMouseMovement(double xpos_a, double ypos_a) override
	{
		float xpos = static_cast<float>(xpos_a);
		float ypos = static_cast<float>(ypos_a);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		Camera.ProcessEye(xoffset, yoffset);
	}

	void Update(float deltatime) override
	{
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::D))
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		Camera.Update();

	}

	void mRenderscene()
	{
		Graphics::API::Context::Clear(Graphics::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		VShader.Bind();
		PShader.Bind();
		LinearSampler.PSBind(0);
		CubeTex.PSBind(0);

		CubeVB.Bind();

		// cube 1
		Math::Matrix4 CubeModel;
		CubeModel = Math::Translate(CubeModel, Math::Vector3(-1.0f, 0.0f, -1.0f));
		Camera.SetModelMatrix(CubeModel);
		Graphics::API::Context::Draw(36);
		// cube 2
		CubeModel = Math::Matrix4();
		CubeModel = Math::Translate(CubeModel, Math::Vector3(2.0f, 0.0f, 0.0f));
		Camera.SetModelMatrix(CubeModel);
		Graphics::API::Context::Draw(36);

		// floor
		PlaneTex.PSBind(0);

		PlaneVB.Bind();
		Camera.SetModelMatrix(Math::Matrix4());
		Graphics::API::Context::Draw(6);
	}

	/*
	Controls:
	1 - Enable Rasterizer State
	2 - Restore Default Rasterizer State

	W - Move Camera Forward
	A - Move Camera Left
	S - Move Camera Backward
	D - Move Camera Right

	Mouse - Make Camera look around
	*/

	void Render(float) override
	{


		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num1))
		{
			mRenderscene();
		}
		
		else {
			Graphics::API::DirectX::DX11Context::GetContext()->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
			float color[4] = { 0.0f,0.0f,1.0f,1.0f };
			Graphics::API::DirectX::DX11Context::GetContext()->ClearRenderTargetView(m_renderTargetView, color);
			Graphics::API::DirectX::DX11Context::GetContext()->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			//Enable Depth Test
			states.EnabledDepth_DisabledStencil.Bind();

			//Render to render-target
			mRenderscene();

			//Bind default RenderTarget
			Graphics::API::DirectX::DX11RenderTarget::Bind_Default();

			Graphics::API::Context::Clear(Graphics::Color(1.0f, 1.0f, 1.0f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

			//Render RT Texture (color buffer) content
			states.DisabledDepthStencil.Bind();

			if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num1))
			{
				R_State.Bind();
			}
			else if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Num2))
			{
				states.DefaultRasterizer.Bind();
			}

			ScreenVShader.Bind();
			ScreenPShader.Bind();
			ScreenQuadVB.Bind();
			ScreenSampler.PSBind(0);
			Graphics::API::DirectX::DX11Context::GetContext()->PSSetShaderResources(0, 1, &m_shaderResourceView);
			Graphics::API::Context::Draw(6);
			PlaneTex.PSBind(0);


			Graphics::API::Context::PresentFrame();
		}
	}
	void Shutdown() override
	{
		//Graphics::API::Shader::Delete(&TestShader);
		Graphics::API::VertexBuffer::Delete(&CubeVB);
		Graphics::API::VertexBuffer::Delete(&PlaneVB);
		Graphics::API::Texture::Delete(&CubeTex);
	}
};

#endif