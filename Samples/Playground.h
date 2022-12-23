#pragma once
#include "SampleBase.h"

//Current TODO:
//Shadow test
class Playground : public SampleBase
{
	Assets::Material RustedIron;
	Assets::Material Plastic;

	Assets::MaterialData RustedIron_D;
	Assets::MaterialData Plastic_D;

	Assets::Font* ArialFont;

	Assets::Shader* PBR;

	Rendering::GeometryPass GeoPass;
	Rendering::PostProcessingPass PostFXPass;
	//Rendering::DefferedPass DefferedPass;
	Rendering::ShadowPass ShadowPass;
public:
	Playground()
	{
	}
	void SetupAssets()
	{
		//Load some textures manually
		Assets::ImageLoadingDesc desc;
		//desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet PBRRustedIron;
		PBRRustedIron.mData.push_back({ 0, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/RustedIron/albedo.png",{ Graphics::TextureUsageType::Diffuse } ) });
		PBRRustedIron.mData.push_back({ 1, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/RustedIron/metallic.png", {Graphics::TextureUsageType::Specular}) });
		PBRRustedIron.mData.push_back({ 2, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/RustedIron/normal.png",{ Graphics::TextureUsageType::Normal}) });
		PBRRustedIron.mData.push_back({ 3, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/RustedIron/roughness.png", { Graphics::TextureUsageType::Roughness}) });
		PBRRustedIron.mData.push_back({ 4, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/RustedIron/ao.png", { Graphics::TextureUsageType::AO}) });

		RustedIron_D.mTextures.push_back(PBRRustedIron);
		RustedIron.SetName("RustedIron Material");

		Assets::TextureSet PBRPlastic;
		PBRPlastic.mData.push_back({ 0, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/plastic/albedo.png",{ Graphics::TextureUsageType::Diffuse}) });
		PBRPlastic.mData.push_back({ 1, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/plastic/metallic.png", {Graphics::TextureUsageType::Specular}) });
		PBRPlastic.mData.push_back({ 2, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/plastic/normal.png",{ Graphics::TextureUsageType::Normal}) });
		PBRPlastic.mData.push_back({ 3, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/plastic/roughness.png", { Graphics::TextureUsageType::Roughness}) });
		PBRPlastic.mData.push_back({ 4, GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/plastic/ao.png", { Graphics::TextureUsageType::AO}) });

		Plastic_D.mTextures.push_back(PBRPlastic);
		Plastic.SetName("Plastic Material");

		RustedIron.Create(&RustedIron_D, PBR);
		Plastic.Create(&Plastic_D, PBR);

		Assets::FontLoadingDesc fdesc;
		ArialFont = GetAssetManager().ImportFont("@CommonAssets@/Fonts/arial.ttf", fdesc);

		//Test asset metadata
		auto test = GetAssetManager().ImportTexture("@CommonAssets@/Textures/PBR/plastic/albedo.png", { Graphics::TextureUsageType::Diffuse });
		auto meta = GetAssetManager().CreateMetadata(test.GetImage());
		Serialization::SerializationEngine::GetInstance().Serialize(meta, "Asset.meta");
		Assets::AssetMetadata META2;
		Serialization::SerializationEngine::GetInstance().Deserialize(META2, "Asset.meta");

	}
	void SetupEntities()
	{
		//Create Entities
		auto EDirLight = GetScene().CreateEntity("DirLight");
		auto& dircomp = EDirLight.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Directional);
		dircomp.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));

		auto ELights = GetScene().CreateEntity("PointLight1");
		auto& lightcomp = ELights.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);
		lightcomp.mCastShadows = true;

		auto ELights2 = GetScene().CreateEntity("PointLight2");
		auto& light2comp = ELights2.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);
		light2comp.mCastShadows = true;

		//Assign Components
		EController.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Spot);

		ELights.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Math::Vector3(-2.0f, 4.0f, -1.0f));
	}


	void InitRenderer()
	{
		Assets::ShaderLoadingDesc desc;
		desc.mType = Assets::ShaderType::_3DRendering;
		PBR = GetAssetManager().ImportShader("@NuclearAssets@/Shaders/PBR/PBR.NEShader", desc);
		Renderer->RegisterShader(PBR);

		Renderer->AddRenderPass(&ShadowPass);
		Renderer->AddRenderPass(&GeoPass);
		//Renderer->AddRenderPass(&DefferedPass);
		Renderer->AddRenderPass(&PostFXPass);

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);
		PostFXPass.Bake({ _Width_, _Height_,Rendering::RenderingEngine::GetInstance().GetFinalRT().GetDesc() });
	}

	void Load()
	{
		SampleBase::Load();
		Rendering::ShadowPassBakingDesc spdesc;

		spdesc.MAX_OMNIDIR_CASTERS =2;
		spdesc.MAX_SPOT_CASTERS = 0;
		ShadowPass.Bake(spdesc);

		SetupEntities();

		InitRenderer();

		SetupAssets();


		// cubes
		{
			ECS::Transform TSphere;
			TSphere.SetPosition(Math::Vector3(0.0f, 1.5f, 0.0));
		//	TSphere.SetScale(Math::Vector3(0.5f));
			GetScene().CreateBox(&RustedIron, TSphere);
		}
		{		
			ECS::Transform TSphere;

			TSphere.SetPosition(Math::Vector3(2.0f, 0.0f, 1.0));
		//	TSphere.SetScale(Math::Vector3(0.5f));
			GetScene().CreateBox(&RustedIron, TSphere);
		}
		{		
			ECS::Transform TSphere;

			TSphere.SetPosition(Math::Vector3(-1.0f, 0.0f, 2.0));
			TSphere.SetRotation(glm::normalize(glm::vec3(1.0, 0.0, 1.0)), glm::radians(60.0f));
	//		TSphere.SetScale(Math::Vector3(0.25));
			GetScene().CreateBox(&RustedIron, TSphere);
		}
		GetScene().CreatePlane(&Plastic);

		GetScene().GetMainCamera()->mRTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);
		//GetScene().GetMainCamera()->MovementSpeed = 15;
		//Renderer->VisualizePointLightsPositions = true;
		//EController.GetComponent<Components::SpotLightComponent>().mCastShadows = true;

		Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
	}
	bool iskinematic = false;

	void Render(float dt) override
	{
		GetScene().Update(dt);
		static bool LockSpotlight = true;

		if (LockSpotlight)
		{
			EController.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(GetScene().GetMainCamera()->GetPosition());
			EController.GetComponent<Components::LightComponent>().SetDirection(GetScene().GetMainCamera()->GetFrontView());
		}
		{
			using namespace Graphics;
			ImGui::Begin("PlayGround : Testing new features");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			//ImGui::Image(ArialFont->mTextureView, {256.f,256.f });

			ImGui::ColorEdit3("Camera ClearColor", (float*)&GetScene().GetMainCamera()->mRTClearColor);

			ImGui::Checkbox("ShowRegisteredRenderTargets", &DebugSystem->ShowRegisteredRenderTargets);
			ImGui::Checkbox("LockSpotlight", &LockSpotlight);
			ImGui::Checkbox("RenderLightSources", &DebugSystem->RenderLightSources);

			if (ImGui::TreeNode("PostFX Effects"))
			{
				for (auto& it : PostFXPass.GetPipelineController().GetSwitches())
				{
					bool value = it.second.GetValue();
					ImGui::Checkbox(it.second.GetName().c_str(), &value);
					if (value != it.second.GetValue())
					{
						PostFXPass.SetPostProcessingEffect(it.second.GetID(), value);
					}
				}
				ImGui::TreePop();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Core::Engine::GetInstance().EndClient();
			}

			ImGui::End();
			EntityExplorer();
		}
	}

	void Shutdown() override
	{
		GetAssetManager().FlushContainers();
	}
};