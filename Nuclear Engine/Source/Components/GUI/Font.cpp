#include "..\..\..\include\Components\GUI\Font.h"
#include <API/Shader.h>
#include <API/ShaderCompiler.h>
#include <API/RenderStates_Types.h>
#include <Core/Context.h>
#include <Core/FileSystem.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma comment (lib, "freetype271MT.lib")

namespace NuclearEngine
{
	namespace Components
	{
		namespace GUI
		{

			Font::Font()
			{
			}
			Font::~Font()
			{
			}
			bool Font::Create(std::string path, unsigned int size, std::string VertexShaderPath, std::string PixelShaderPATH)
			{ 
				Log.Info("[Font] Loading Font: " + path + "\n");
				
				API::ShaderDesc desc;
				desc.Name = "TextShader";
				desc.VertexShaderCode.convertedshaderrowmajor = true;
				API::CompileShader(&desc.VertexShaderCode, Core::FileSystem::LoadFileToString(VertexShaderPath).c_str(), API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
				API::CompileShader(&desc.PixelShaderCode, Core::FileSystem::LoadFileToString(PixelShaderPATH).c_str(), API::ShaderType::Pixel, API::ShaderLanguage::HLSL);
				API::Shader::Create(&TextShader, &desc);
								
				API::ConstantBuffer::Create(&TextColorCB, "NE_TextColor_CB", sizeof(Math::Vector4));
				TextShader.SetConstantBuffer(&TextColorCB, API::ShaderType::Pixel);

				API::ConstantBuffer::Create(&TextProjectionCB, "NE_Projection_CB", sizeof(Math::Matrix4));
				TextShader.SetConstantBuffer(&TextProjectionCB, API::ShaderType::Vertex);

				API::Texture_Desc TexDesc;
				TexDesc.Filter = API::TextureFilter::Linear2D;
				TexDesc.Wrap = API::TextureWrap::ClampToEdge;
				TexDesc.Format = API::Format::R8;
				TexDesc.Type = API::TextureType::Texture2D;

				API::BlendStateDesc blenddesc;
				blenddesc.RenderTarget[0].BlendEnable = true;
				blenddesc.RenderTarget[0].SrcBlend = API::BLEND::SRC_ALPHA;
				blenddesc.RenderTarget[0].DestBlend = API::BLEND::INV_SRC_ALPHA;
				blenddesc.RenderTarget[0].SrcBlendAlpha = API::BLEND::SRC_ALPHA;
				blenddesc.RenderTarget[0].DestBlendAlpha = API::BLEND::INV_SRC_ALPHA;
				blenddesc.RenderTarget[0].RenderTargetWriteMask = API::COLOR_WRITE_ENABLE_ALL;
				API::BlendState::Create(&Blending_State, blenddesc);

				// FreeType
				FT_Library ft;
				// All functions return a value different than 0 whenever an error occurred
				if (FT_Init_FreeType(&ft))
				{
					Log.Error("[Font] Couldn't initialize Freetype Library!\n");
					return false;
				}
				// Load font as face
				FT_Face face;
				if (FT_New_Face(ft, path.c_str(), 0, &face))
				{
					Log.Error("[Font] Couldn't load Font!\n");
					return false;
				}
				// Set size to load glyphs as
				FT_Set_Pixel_Sizes(face, 0, 48);


				// Load first 128 characters of ASCII set
				for (unsigned char c = 0; c < 128; c++)
				{
					// Load character glyph 
					if (FT_Load_Char(face, c, FT_LOAD_RENDER))
					{
						continue;
					}

					API::Texture texture;
					API::Texture_Data data;
					data.Width = face->glyph->bitmap.width;
					data.Height = face->glyph->bitmap.rows;
					data.Img_Data_Buf = face->glyph->bitmap.buffer;

					API::Texture::Create(&texture, data, TexDesc);
					// Now store character for later use
					Character character = {
						texture,
						Math::Vector2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
						Math::Vector2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
						face->glyph->advance.x
					};
					Characters.insert(std::pair<char, Character>(c, character));

				}

				// Destroy FreeType once we're finished
				FT_Done_Face(face);
				FT_Done_FreeType(ft);

				// Configure VAO/VBO for texture quads
				VertexBufferDesc Desc;
				Desc.data = NULL;
				Desc.size = sizeof(float) * 6 * 4;
				Desc.usage = BufferUsage::Dynamic;

				API::VertexBuffer::Create(&Font_VB, &Desc);

				API::InputLayout Inputlayout;
				Inputlayout.Push("POSITION", 0, DataType::Float4);
				Font_VB.SetInputLayout(&Inputlayout, &TextShader);
				return true;
			}
		
			void Font::SetProjectionMatrix(Math::Matrix4 projection)
			{
				TextProjectionCB.Update(&projection, sizeof(Math::Matrix4));
			}

			void Font::RenderText(std::string text, float x, float y, float scale, API::Color color)
			{
				// Activate corresponding render state	
				Blending_State.Bind();
				TextShader.Bind();
				Math::Vector4 cbcolor(color.r, color.g, color.b, 1.0f);
				TextColorCB.Update(&cbcolor, sizeof(cbcolor));
				Font_VB.Bind();
				// Iterate through all characters
				std::string::const_iterator c;
				for (c = text.begin(); c != text.end(); c++)
				{
					Character ch = Characters[*c];
					GLfloat xpos = x + ch.Bearing.x * scale;
					GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

					GLfloat w = ch.Size.x * scale;
					GLfloat h = ch.Size.y * scale;
					// Update VBO for each character
					GLfloat vertices[6][4] = {
					{ xpos,     ypos + h,   0.0, 1.0 },
					{ xpos + w, ypos,       1.0, 0.0 },
					{ xpos,     ypos,       0.0, 0.0 },

					{ xpos,     ypos + h,   0.0, 1.0 },
					{ xpos + w, ypos + h,   1.0, 1.0 },
					{ xpos + w, ypos,       1.0, 0.0 }
					};

					// Render glyph texture over quad
					ch.Texture.PSBind("NE_TextTexture", &TextShader, 0);
					// Update content of VBO memory
					Font_VB.Update(vertices, sizeof(vertices));
					// Render quad
					Core::Context::Draw(6);
					// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
				}
			}
		}
	}
}