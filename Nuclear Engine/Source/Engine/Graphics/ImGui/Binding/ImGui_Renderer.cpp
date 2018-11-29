#include "Engine\Graphics\ImGui_Renderer.h"
#include "Core/FileSystem.h"
#include <Engine/Graphics/API/OpenGL\GLContext.h>

namespace NuclearEngine
{
	namespace Graphics
	{
	
		static API::VertexBuffer       gVB;
		static API::IndexBuffer        gIB;
		static API::VertexShader       gVertexShader;
		static API::InputLayout        gInputLayout;
		static API::ConstantBuffer     gVertexConstantBuffer;
		static API::PixelShader		   gPixelShader;
		static API::Sampler			   gFontSampler;
		static API::Texture			   gFontTexture;
		static API::RasterizerState	   gRasterizerState;
		static API::BlendState		   gBlendState;
		static API::DepthStencilState  gDepthStencilState;
		static int  g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;
		static bool Initialized = false;
		bool ImGui_Renderer::Initialize()
		{
			if (Initialized == true)
			{
				Shutdown();
			}

			// Create the vertex shader
			Graphics::API::VertexShader::Create(
				&gVertexShader,
				Graphics::API::CompileShader(
					Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/GUIShader.vs.hlsl"),
					Graphics::API::ShaderType::Vertex));

			// Create the input layout
			gInputLayout.AppendAttribute("POSITION", 0, Graphics::API::Format::R32G32_FLOAT);
			gInputLayout.AppendAttribute("COLOR", 0, Graphics::API::Format::R8G8B8A8_UNORM);
			gInputLayout.AppendAttribute("TEXCOORD", 0, Graphics::API::Format::R32G32_FLOAT);

			gInputLayout.AutoAlignedOffset = false;

			// Create the constant buffer
			API::ConstantBuffer::Create(&gVertexConstantBuffer, "vertexBuffer", sizeof(float[4][4]));
		
			gVertexShader.SetConstantBuffer(&gVertexConstantBuffer);

			// Create the pixel shader
			Graphics::API::PixelShader::Create(
				&gPixelShader,
				Graphics::API::CompileShader(
					Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/GUIShader.ps.hlsl"),
					Graphics::API::ShaderType::Pixel));


			// Create the blending setup
			{
				API::BlendStateDesc desc;
				desc.AlphaToCoverageEnable = false;
				desc.RenderTarget[0].BlendEnable = true;
				desc.RenderTarget[0].SrcBlend = API::BLEND::SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = API::BLEND::INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp = API::BLEND_OP::OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = API::BLEND::INV_SRC_ALPHA;
				desc.RenderTarget[0].DestBlendAlpha = API::BLEND::ZERO;
				desc.RenderTarget[0].BlendOpAlpha = API::BLEND_OP::OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = API::COLOR_WRITE_ENABLE_ALL;
				API::BlendState::Create(&gBlendState, desc);
			}

			// Create the rasterizer state
			{
				API::RasterizerStateDesc desc;
				desc.FillMode = API::FillMode::Solid;
				desc.CullMode = API::CullMode::None;
				desc.ScissorEnable = true;
				desc.DepthClipEnable = true;
				API::RasterizerState::Create(&gRasterizerState, desc);
			}

			// Create depth-stencil State
			{
				API::DepthStencilStateDesc desc;
				desc.DepthEnabled = false;
				desc.DepthMaskEnabled = true;
				desc.DepthFunc = API::Comparison_Func::ALWAYS;
				desc.StencilEnabled = false;
				desc.StencilFrontFace.StencilFailOp = desc.StencilFrontFace.StencilDepthFailOp = desc.StencilFrontFace.StencilPassOp = API::Stencil_OP::KEEP;
				desc.StencilFrontFace.StencilFunc = API::Comparison_Func::ALWAYS;
				desc.StencilBackFace = desc.StencilFrontFace;
				API::DepthStencilState::Create(&gDepthStencilState, desc);
			}

			CreateFontsTexture();
			Initialized = true;
			Log.Info("[ImGui] Initialized ImGui_Renderer.\n");
			return true;
		}

		void ImGui_Renderer::Shutdown()
		{
			Log.Info("[ImGui] Shutting down ImGui_Renderer...\n");
			API::VertexBuffer::Delete(&gVB);
			API::IndexBuffer::Delete(&gIB);
			API::VertexShader::Delete(&gVertexShader);
			API::ConstantBuffer::Delete(&gVertexConstantBuffer);
			API::PixelShader::Delete(&gPixelShader);
			API::Sampler::Delete(&gFontSampler);
			API::Texture::Delete(&gFontTexture);
			API::RasterizerState::Delete(&gRasterizerState);
			API::BlendState::Delete(&gBlendState);
			API::DepthStencilState::Delete(&gDepthStencilState);
		}
		void ImGui_Renderer::NewFrame()
		{
		}
		static bool ValidVB = false, ValidIB = false;
		void ImGui_Renderer::RenderDrawData(ImDrawData * draw_data)
		{
			// Create and grow vertex/index buffers if needed
			if (ValidVB == false || g_VertexBufferSize < draw_data->TotalVtxCount)
			{
				API::VertexBuffer::Delete(&gVB);

				g_VertexBufferSize = draw_data->TotalVtxCount + 5000;

				API::VertexBufferDesc desc;
				desc.UsageType = API::BufferUsage::Dynamic;
				desc.Size = g_VertexBufferSize * sizeof(ImDrawVert);
				desc.Data = NULL;
				API::VertexBuffer::Create(&gVB, desc);

				gVB.SetInputLayout(&gInputLayout, &gVertexShader);
				ValidVB = true;
			}
			if (ValidIB == false || g_IndexBufferSize < draw_data->TotalIdxCount)
			{
				API::IndexBuffer::Delete(&gIB);

				g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
				API::IndexBufferDesc desc;
				desc.UsageType = API::BufferUsage::Dynamic;
				desc.Size = g_IndexBufferSize * sizeof(ImDrawIdx);
				desc.Data = NULL;
				desc.UsePreciseSize = true;
				API::IndexBuffer::Create(&gIB, desc);
				ValidIB = true;
			}

			ImDrawVert* vtx_dst = (ImDrawVert*)gVB.Map();
			ImDrawIdx* idx_dst = (ImDrawIdx*)gIB.Map();
			for (int n = 0; n < draw_data->CmdListsCount; n++)
			{
				const ImDrawList* cmd_list = draw_data->CmdLists[n];
				memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
				memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
				vtx_dst += cmd_list->VtxBuffer.Size;
				idx_dst += cmd_list->IdxBuffer.Size;
			}
			gVB.Unmap();
			gIB.Unmap();

			// Setup orthographic projection matrix into our constant buffer
			// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). 

			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

			if (Graphics::API::Context::isOpenGL3RenderAPI())
			{
				float mvp[4][4] =
				{
					{ 2.0f/(R-L),   0.0f,         0.0f,   0.0f },
					{ 0.0f,         2.0f/(T-B),   0.0f,   0.0f },
					{ 0.0f,         0.0f,        -1.0f,   0.0f },
					{ (R+L)/(L-R),  (T+B)/(B-T),  0.0f,   1.0f },
				};

				gVertexConstantBuffer.Update(mvp, sizeof(mvp));
			}
			else if (Graphics::API::Context::isDirectX11RenderAPI())
			{
				float mvp[4][4] =
				{
					{ 2.0f/(R-L),   0.0f,           0.0f,       0.0f },
					{ 0.0f,         2.0f/(T-B),     0.0f,       0.0f },
					{ 0.0f,         0.0f,           0.5f,       0.0f },
					{ (R+L)/(L-R),  (T+B)/(B-T),    0.5f,       1.0f },
				};

				gVertexConstantBuffer.Update(mvp, sizeof(mvp));
			}

			// Backup state that will be modified to restore it afterwards 
			auto state = Graphics::API::Context::SaveState();

			if (sizeof(ImDrawIdx) == 2)
				Graphics::API::Context::SetIndicesType(Graphics::API::IndicesFormat::UINT_R16);
			else
				Graphics::API::Context::SetIndicesType(Graphics::API::IndicesFormat::UINT_R32);

			gVB.Bind();
			gIB.Bind();

			// Setup viewport
			Graphics::API::Context::SetViewPort(0, 0, draw_data->DisplaySize.x, draw_data->DisplaySize.y);			

			// Bind shader
			Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
			gVertexShader.Bind();
			gPixelShader.Bind();
			gFontSampler.PSBind(0);

			gBlendState.Bind(Graphics::Color(0.f, 0.f, 0.f, 0.f), 0xffffffff);
			gDepthStencilState.Bind();
			gRasterizerState.Bind();

			ImGuiIO& io = ImGui::GetIO();
			int fb_width = (int)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
			int fb_height = (int)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);
			if (fb_width <= 0 || fb_height <= 0)
				return;

			// Render command lists
			int vtx_offset = 0;
			int idx_offset = 0;
			ImVec2 pos = draw_data->DisplayPos;
			for (int n = 0; n < draw_data->CmdListsCount; n++)
			{
				const ImDrawList* cmd_list = draw_data->CmdLists[n];

				for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
				{
					const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
					if (pcmd->UserCallback)
					{
						// User callback (registered via ImDrawList::AddCallback)
						pcmd->UserCallback(cmd_list, pcmd);
					}
					else
					{
						// Apply scissor/clipping rectangle
					
						ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
						if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
						{
							// Apply scissor/clipping rectangle
							Graphics::API::Context::SetScissors((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
							
							// Bind texture, Draw
							gFontTexture.PSBind(0);
							Graphics::API::Context::DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
						}

					
					}
					idx_offset += pcmd->ElemCount;
				}
				vtx_offset += cmd_list->VtxBuffer.Size;
			}

			Graphics::API::Context::LoadState(state);
		}

		void ImGui_Renderer::CreateFontsTexture()
		{
			// Build texture atlas
			ImGuiIO& io = ImGui::GetIO();
			unsigned char* pixels;
			int width, height;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

			// Upload texture to graphics system

			Graphics::API::Texture_Data data;
			data.Width = width;
			data.Height = height;
			data.Img_Data_Buf = pixels;

			Graphics::API::Texture_Desc desc;
			desc.GenerateMipMaps = false;
			desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;

			API::Texture::Create(&gFontTexture, data, desc);

			// Store our identifier
			io.Fonts->TexID = &gFontTexture;

			API::SamplerDesc samplerdesc;

			samplerdesc.Filter = API::TextureFilter::Trilinear;
			samplerdesc.WrapU = API::TextureWrap::Repeat;
			samplerdesc.WrapV = API::TextureWrap::Repeat;
			samplerdesc.WrapW = API::TextureWrap::Repeat;
		
			API::Sampler::Create(&gFontSampler, samplerdesc);
		}
		void ImGui_Renderer::DestroyFontsTexture()
		{
			API::Texture::Delete(&gFontTexture);
			API::Sampler::Delete(&gFontSampler);
		}
	}
}

