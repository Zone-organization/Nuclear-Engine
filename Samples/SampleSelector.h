#pragma once
#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"
#include "Sample4.h"
#include "Sample5.h"
#include "SponzaDemo.h"
#include "Playground.h"

class SampleSelector : public Core::Client
{
public:

	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetInstance().GetSwapChain()->Resize(width, height);
	}
	SampleSelector()
	{
		//	Instance = this;
	}
	void StartSample(Client* sample)
	{
		ImGui::End();
		Core::Engine::GetInstance().EndFrame();
		Core::Engine::GetInstance().EndClient();

		Core::Engine::GetInstance().LoadClient(sample);
		Core::Engine::GetInstance().EndClient();
	}

	void ImportModels()
	{
		Assets::ModelImportingDesc ModelDesc;

		GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/CrytekNanosuit/nanosuit.obj", ModelDesc);

		GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/CrytekCyborg/cyborg.obj", ModelDesc);

		GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/Bob/boblampclean.md5mesh", ModelDesc);

		GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/vampire/vampire_a_lusth.fbx", ModelDesc);

		ModelDesc.LoadMaterialInfo = false;
		ModelDesc.LoadAnimation = false;

		GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/shaderball/shaderball.obj", ModelDesc);
	}

	void ImportShaders()
	{
		Assets::ShaderImportingDesc desc;
		desc.mType = Graphics::ShaderType::_3DRendering;
		GetAssetManager().Import<Assets::Shader>("@NuclearAssets@/Shaders/BlinnPhong.NuclearShader", desc);
		GetAssetManager().Import<Assets::Shader>("@NuclearAssets@/Shaders/DiffuseOnly.NuclearShader", desc);
	}

	void ImportPBRAssets()
	{
		Assets::Shader* PBR;
		Assets::ShaderImportingDesc desc;
		desc.mType = Graphics::ShaderType::_3DRendering;
		PBR = GetAssetManager().Import<Assets::Shader>("@NuclearAssets@/Shaders/PBR/PBR.NuclearShader", desc);

		Assets::Material RustedIron;
		Assets::Material Plastic;
		Assets::Material Grass;
		Assets::Material Gold;
		Assets::Material Wall;

		Assets::MaterialTextureSet PBRRustedIron;
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/albedo.png") , Assets::TextureUsageType::Diffuse });
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/metallic.png") , Assets::TextureUsageType::Specular });
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/normal.png") , Assets::TextureUsageType::Normal });
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/roughness.png") ,  Assets::TextureUsageType::Roughness });
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/ao.png") ,  Assets::TextureUsageType::AO });

		Assets::MaterialCreationInfo RustedIron_D;
		RustedIron_D.mTextures.push_back(PBRRustedIron);
		RustedIron.SetName("RustedIron");

		Assets::MaterialTextureSet PBRPlastic;
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/albedo.png") , Assets::TextureUsageType::Diffuse });
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/metallic.png") , Assets::TextureUsageType::Specular });
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/normal.png") , Assets::TextureUsageType::Normal });
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/roughness.png") ,  Assets::TextureUsageType::Roughness });
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/ao.png") ,  Assets::TextureUsageType::AO });

		Assets::MaterialCreationInfo Plastic_D;
		Plastic_D.mTextures.push_back(PBRPlastic);
		Plastic.SetName("Plastic");

		Assets::MaterialTextureSet PBRGrass;
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/albedo.png") , Assets::TextureUsageType::Diffuse });
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/metallic.png") , Assets::TextureUsageType::Specular });
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/normal.png") , Assets::TextureUsageType::Normal });
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/roughness.png") ,  Assets::TextureUsageType::Roughness });
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/ao.png") ,  Assets::TextureUsageType::AO });

		Assets::MaterialCreationInfo Grass_D;
		Grass_D.mTextures.push_back(PBRGrass);
		Grass.SetName("Grass");

		Assets::MaterialTextureSet PBRWall;
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/albedo.png") , Assets::TextureUsageType::Diffuse });
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/metallic.png") , Assets::TextureUsageType::Specular });
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/normal.png") , Assets::TextureUsageType::Normal });
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/roughness.png") ,  Assets::TextureUsageType::Roughness });
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/ao.png") ,  Assets::TextureUsageType::AO });

		Assets::MaterialCreationInfo Wall_D;
		Wall_D.mTextures.push_back(PBRWall);
		Wall.SetName("Wall");

		Assets::MaterialTextureSet PBRGold;
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/albedo.png") , Assets::TextureUsageType::Diffuse });
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/metallic.png") , Assets::TextureUsageType::Specular });
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/normal.png") , Assets::TextureUsageType::Normal });
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/roughness.png") ,  Assets::TextureUsageType::Roughness });
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/ao.png") ,  Assets::TextureUsageType::AO });

		Assets::MaterialCreationInfo Gold_D;
		Gold_D.mTextures.push_back(PBRGold);
		Gold.SetName("Gold");

		RustedIron.Create(RustedIron_D, PBR);
		Plastic.Create(Plastic_D, PBR);
		Grass.Create(Grass_D, PBR);
		Wall.Create(Wall_D, PBR);
		Gold.Create(Gold_D, PBR);


		//Export Materials
		GetAssetManager().Export(&RustedIron);
		GetAssetManager().Export(&Plastic);
		GetAssetManager().Export(&Grass);
		GetAssetManager().Export(&Wall);
		GetAssetManager().Export(&Gold);
	}

	void Render(float dt) override
	{
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		auto* RTV = Graphics::Context::GetInstance().GetSwapChain()->GetCurrentBackBufferRTV();
		auto* DSV = Graphics::Context::GetInstance().GetSwapChain()->GetDepthBufferDSV();

		Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, &RTV, DSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(RTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetInstance().GetContext()->ClearDepthStencil(DSV, Diligent::CLEAR_DEPTH_FLAG, 1.0f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		using namespace Graphics;
		ImGui::Begin("SampleSelector");

		ImGui::Text("Select Sample");

		if (ImGui::Button("Sample1 : Basic model loading & animation & rendering"))
		{
			Sample1 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample2 : Introduction to rendering pipelines and shading models."))
		{
			Sample2 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample3 : PhysX & Scripting Test WIP"))             //Improve
		{
			Sample3 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample4 : Multiple Cameras"))
		{
			Sample4 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample5 : Async Asset Importing"))
		{
			Sample5 demo;
			return StartSample(&demo);
		}

		ImGui::Text("Select Demo");

		if (ImGui::Button("Sponza Demo"))
		{
			SponzaDemo demo;
			return StartSample(&demo);
		}

		if (ImGui::Button("Playground"))
		{
			Playground demo;
			return StartSample(&demo);
		}

		if (ImGui::Button("Build Assets Library"))
		{
			ImportPBRAssets();
			
			SponzaDemo demo;
			return StartSample(&demo);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
};