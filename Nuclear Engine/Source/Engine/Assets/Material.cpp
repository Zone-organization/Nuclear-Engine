#include <Engine\Assets\Material.h>
#include <Base\Utilities\Hash.h>
#include <Engine/Assets/DefaultTextures.h>
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Assets
	{	
		Material::Material()
			: mPixelShaderTextures(std::vector<TextureSet>()), PixelShaderTS(std::vector<ShaderTexture>())
		{
		}
		Material::~Material()
		{
		}
		void Material::Create(const MaterialCreationDesc& desc)
		{
			//Process Shaders & pipeline
			ParsePixelShader(desc.mPShader);
			///
			//mPipeline = desc.mPipeline;

			/*TODO:
				1: Generate PipelineLayout based on	ShaderReflection.
				2: Create ResourceHeap for each texture.
			*/
			//LLGL::ResourceHeapDescriptor;
			

			/*for (auto TexSet : mPixelShaderTextures)
			{
				for (auto Tex : TexSet)
				{
					LLGL::ResourceHeapDescriptor resourceHeapDesc;
					{
						resourceHeapDesc.pipelineLayout = pipelineLayout;
						resourceHeapDesc.resourceViews = { sampler, colorMap };
					}
					resourceHeaps[i] = renderer->CreateResourceHeap(resourceHeapDesc);
					Tex.mResourceHeap;
				}
			}*/
		}


		void Material::BindTexSet(Uint32 index)
		{
			//TODO: Check if all Slots have been occupied and then bind the free ones to fix some glitches
			if (!mPixelShaderTextures.empty())
			{
				for (auto tex : mPixelShaderTextures.at(index))
				{	
					//BINDING_LLGL
					//tex.mTexture.PSBind(tex.mSlot);
				}
			}
		}

		void Material::Bind()
		{	
			//BINDING_LLGL
			//mCbuffer.PSBind(mCbufferRef.BindingSlot);
		}
		void Material::UpdateMaterialCBuffer()
		{
		/*	std::vector<Float32> data;
			data.reserve(mCbufferData.size());
			for (auto i : mCbufferData)
			{				
				data.push_back(i.second);
			}
			Graphics::Context::GetRenderer()->WriteBuffer(*mCbuffer, 0, data.data(), mCbufferRef.mSize);*/
		}
	
		TextureUsageType ParseTexUsageFromName(std::string& name)
		{
			if (name.find("Diffuse") == 0)
				return TextureUsageType::Diffuse;
			else if (name.find("Specular") == 0)
				return TextureUsageType::Specular;
			else if (name.find("Normal") == 0)
				return TextureUsageType::Normal;

			return TextureUsageType::Unknown;
		}

		void Material::ParsePixelShader(IShader* _PShader)
		{/*
			if (_PShader->mShader)
			{
				mPSHaveMaterialCB = false;

				PixelShaderTS.clear();
				for (auto Resource : _PShader->mReflection.mResources)
				{
					//Parse Textures
					if (Graphics::ShaderTypesHelpers::IsTexture(Resource.second.mType))
					{
						if (Resource.first.find("NEMat_") == 0)
						{
							std::string texname(Resource.first);
							texname.erase(0, 7);
							ShaderTexture mTex;
							mTex.mTexture = &DefaultTextures::DefaultBlackTex;
							mTex.mTexture->SetName(texname);
							mTex.mSlot = Resource.second.mSlot;
							mTex.mTexture->SetUsageType(ParseTexUsageFromName(texname));
							PixelShaderTS.push_back(mTex);
						}
					}

					//Parse Material ConstantBuffer
					auto MatCB = _PShader->mReflection.mResources.find("NEMaterial");
					if (MatCB != _PShader->mReflection.mResources.end())
					{
						mCbufferRef = MatCB->second;
						mPSHaveMaterialCB = true;

						if (mCbuffer)						
							Graphics::Context::GetRenderer()->Release(*mCbuffer);
						
						mCbuffer = Graphics::Context::GetRenderer()->CreateBuffer(LLGL::ConstantBufferDesc(mCbufferRef.mSize));

						//Parse Material Variables
						for (auto var : mCbufferRef.mVariables)
						{
							using namespace Graphics;
							switch (var.second.mType)
							{
							case ShaderVariableType::Float1:
								mCbufferData[var.first] = 0.0f;
								break;
							case ShaderVariableType::Float2:
								mCbufferData[var.first + "_1"] = 0.0f;
								mCbufferData[var.first + "_2"] = 0.0f;
								break;
							case ShaderVariableType::Float3:
								mCbufferData[var.first + "_1"] = 0.0f;
								mCbufferData[var.first + "_2"] = 0.0f;
								mCbufferData[var.first + "_3"] = 0.0f;
								break;
							case ShaderVariableType::Float4:
								mCbufferData[var.first + "_1"] = 0.0f;
								mCbufferData[var.first + "_2"] = 0.0f;
								mCbufferData[var.first + "_3"] = 0.0f;
								mCbufferData[var.first + "_4"] = 0.0f;
								break;
							case ShaderVariableType::Matrix2x2:
								for (Uint8 i = 1; i < 3; i++)
								{
									for (Uint8 j = 1; j < 3; j++)
									{
										mCbufferData[var.first + "_" + std::to_string(i) + 'x' + std::to_string(j)] = 0.0f;
									}
								}
								break;
							case ShaderVariableType::Matrix3x3:
								for (Uint8 i = 1; i < 4; i++)
								{
									for (Uint8 j = 1; j < 4; j++)
									{
										mCbufferData[var.first + "_" + std::to_string(i) + 'x' + std::to_string(j)] = 0.0f;
									}
								}
								break;
							case ShaderVariableType::Matrix4x4:
								for (Uint8 i = 1; i < 5; i++)
								{
									for (Uint8 j = 1; j < 5; j++)
									{
										mCbufferData[var.first + "_" + std::to_string(i) + 'x' + std::to_string(j)] = 0.0f;
									}
								}
								break;

							default:
								Log.Error("[Material] Parsing Material Variable \"" + var.first + "\" Of Type: " + std::to_string((int)var.second.mType) + "\n");
							}
						}

						return;
					}
				}

				//Parse loaded textures
				for (size_t i = 0; i < mPixelShaderTextures.size(); i++)
				{
					for (size_t j = 0; j < mPixelShaderTextures.at(i).size(); j++)
					{
						for (ShaderTexture TSinfo : PixelShaderTS)
						{
							if (mPixelShaderTextures.at(i).at(j).mTexture->GetUsageType() == TSinfo.mTexture->GetUsageType())
							{
								mPixelShaderTextures.at(i).at(j).mSlot = TSinfo.mSlot;
							}
						}
					}				
				}		
			}
			*/
		}
	}
}