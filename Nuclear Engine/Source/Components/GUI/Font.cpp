#include "..\..\..\include\Components\GUI\Font.h"
#include <API/Shader.h>
#include <API/ShaderCompiler.h>
#include <API/ConstantBuffer.h>

namespace NuclearEngine
{
	namespace Components
	{
		namespace GUI
		{
			static API::Shader TextShader;
			static API::ConstantBuffer TextCB;
			Font::Font()
			{
			}
			Font::~Font()
			{
			}
			bool Font::Create(std::string path, unsigned int size)
			{ 
				Log->Info("[Font] Loading Font: " + path + "\n");
				

			
				API::Texture_Desc TexDesc;
				TexDesc.Filter = API::TextureFilter::Linear2D;
				TexDesc.Wrap = API::TextureWrap::ClampToEdge;
				TexDesc.Format = API::Format::R8;
				TexDesc.Type = API::TextureType::Texture2D;

				// Set OpenGL options
				glEnable(GL_CULL_FACE);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				// Load first 128 characters of ASCII set
				for (unsigned char c = 0; c < 128; c++)
				{
					//// Load character glyph 
					//if (FT_Load_Char(face, c, FT_LOAD_RENDER))
					//{
					//	continue;
					//}

					API::Texture texture;
					API::Texture_Data data;
					//data.Width = face->glyph->bitmap.width;
					//data.Height = face->glyph->bitmap.rows;
					//data.Img_Data_Buf = face->glyph->bitmap.buffer;
		
					API::Texture::Create(&texture, data, TexDesc);
					// Now store character for later use
					/*Character character = {
						texture,
						Math::Vector2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
						Math::Vector2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
						face->glyph->advance.x
					};
					Characters.insert(std::pair<GLchar, Character>(c, character));*/
				}

				// Destroy FreeType once we're finished
				//FT_Done_Face(face);
				//FT_Done_FreeType(ft);

				// Configure VAO/VBO for texture quads
				VertexBufferDesc Desc;
				Desc.data = NULL;
				Desc.size = sizeof(float) * 6 * 4;
				Desc.usage = BufferUsage::Dynamic;

				API::VertexBuffer::Create(&Font_VB, &Desc);

				API::InputLayout Inputlayout;
				Inputlayout.Push("POSITION", 0, DataType::Float4);
				Font_VB.SetInputLayout(&Inputlayout, nullptr);
			}
			void Font::InitializeTextRenderer(std::string VSPath, std::string PSPATH)
			{
				API::ShaderDesc desc;
				desc.Name = "TextShader";
				API::CompileShader(&desc.VertexShaderCode, VSPath, API::ShaderType::Vertex, API::ShaderLanguage::HLSL);
				API::CompileShader(&desc.PixelShaderCode, PSPATH, API::ShaderType::Pixel, API::ShaderLanguage::HLSL);

				API::Shader::Create(&TextShader, &desc);
				
				API::ConstantBuffer::Create(&TextCB, "NE_TextColor_CB", sizeof(Math::Vector4));
				TextShader.SetConstantBuffer(&TextCB, API::ShaderType::Vertex);
			}
			void Font::RenderText(std::string text, float x, float y, float scale, API::Color color)
			{
				// Activate corresponding render state	
				TextShader.Bind();
				Math::Vector4 cbcolor(color.r, color.g, color.b, 1.0f);
				TextCB.Update(&cbcolor, sizeof(cbcolor));
				Font_VB.Bind();
				// Iterate through all characters
				std::string::const_iterator c;
				for (c = text.begin(); c != text.end(); c++)
				{
					Character ch = Characters[*c];

					float xpos = x + ch.Bearing.x * scale;
					float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

					float w = ch.Size.x * scale;
					float h = ch.Size.y * scale;
					// Update VBO for each character
					float vertices[6][4] = {
						{ xpos,     ypos + h,   0.0, 0.0 },
					{ xpos,     ypos,       0.0, 1.0 },
					{ xpos + w, ypos,       1.0, 1.0 },

					{ xpos,     ypos + h,   0.0, 0.0 },
					{ xpos + w, ypos,       1.0, 1.0 },
					{ xpos + w, ypos + h,   1.0, 0.0 }
					};
					// Render glyph texture over quad
					ch.Texture.PSBind("Character", &TextShader, 0);
					// Update content of VBO memory
					Font_VB.Update(vertices, sizeof(vertices));
					// Render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);
					// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
				}
			}
		}
	}
}