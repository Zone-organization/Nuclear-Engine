#pragma once
#include "SampleBase.h"

class SponzaDemo : public SampleBase
{
	Assets::Material SponzaPBRMaterial;
	Assets::Material SponzaBlinnPhongMaterial;

	Rendering::Skybox Skybox;

	Assets::Shader* PBR;
	Assets::Shader* BlinnPhong;

	///////////////////////////////////////////////////
	Rendering::DefferedRenderingPath DefferedRP;

	Rendering::GeometryPass GeoPass;
	Rendering::PostProcessingPass PostFXPass;
	Rendering::DefferedPass DefferedPass;

	//Rendering::ShadowPass ShadowPass;

	//IBL Settings
	Rendering::ImageBasedLighting IBL;
	Rendering::PBRCapture EnvCapture;
	Graphics::Texture HDREnv;
	Assets::Image HDR_Cube;

	ECS::Entity ESponza;

	ECS::Entity ELights;

	// positions of the point lights
	Math::Vector3 pointLightPositions[9] =
	{
		Math::Vector3(0.7f,  0.2f,  2.0f),
		Math::Vector3(2.3f, -3.3f, -4.0f),
		Math::Vector3(-4.0f,  2.0f, -12.0f),
		Math::Vector3(0.0f,  0.0f, -3.0f),
		Math::Vector3(4.0f,  3.0f, -2.0f),
		Math::Vector3(6.2f, 2.0f, 0.0f),
		Math::Vector3(6.2f, -2.0f, 0.0f),
		Math::Vector3(-6.2f, 2.0f, 0.0f),
		Math::Vector3(-6.2f, -2.0f, 0.0f)
	};
public:
	SponzaDemo()
	{
	}
	void SetupAssets()
	{
		Assets::ModelLoadingDesc ModelDesc;
		ModelDesc.LoadAnimation = false;

	
		//Load Sponza Model
		auto Model = GetAssetManager().Import("@CommonAssets@/Models/CrytekSponza/sponza.fbx", ModelDesc);

		SponzaPBRMaterial.Create(Model.pMaterialData, PBR);
		SponzaBlinnPhongMaterial.Create(Model.pMaterialData, BlinnPhong);

		ESponza.AddComponent<Components::MeshComponent>(Model.pMesh, &SponzaPBRMaterial);

	}
	void SetupEntities()
	{
		//Create Entities
		ECS::Transform TSponza;
		TSponza.SetScale(Math::Vector3(0.05f, 0.05f, 0.05f));
		ESponza = GetScene().CreateEntity("Sponza", TSponza);
		ELights = GetScene().CreateEntity("Lights");


		auto EDirLight = GetScene().CreateEntity("DirLight");
		auto& dircomp = EDirLight.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Directional);
		dircomp.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dircomp.SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		auto ELights = Core::Scene::GetInstance().CreateEntity("PointLight1");
		auto& lightcomp = ELights.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);
		lightcomp.SetIntensity(10.0f);
		lightcomp.mCastShadows = true;

		ELights.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(pointLightPositions[0]);

		//for (int i = 1; i < 9; i++)
		//{
		//	auto Light = GetScene().CreateEntity("Light" + std::to_string(i));
		//	Light.AddComponent<Components::PointLightComponent>();
		//	Light.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(pointLightPositions[i]);
		//	Light.GetComponent<Components::PointLightComponent>().SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		//	Light.GetComponent<Components::PointLightComponent>().SetIntensity(2.f);
		//}

	}

	void InitShaders()
	{
		Assets::ShaderLoadingDesc desc;
		desc.mType = Assets::ShaderType::_3DRendering;
		PBR = GetAssetManager().Import("@NuclearAssets@/Shaders/PBR/PBR.NEShader", desc);
		BlinnPhong = GetAssetManager().Import("@NuclearAssets@/Shaders/BlinnPhong.NEShader", desc);

		Renderer->SetIBLContext(&IBL);
		Renderer->RegisterShader(PBR);
		Renderer->RegisterShader(BlinnPhong);
	}

	void InitIBL()
	{
		//IBL
		Assets::ImageLoadingDesc DESC;
		DESC.mType = RESOURCE_DIM_TEX_2D;
		DESC.mUsage = USAGE_IMMUTABLE;
		DESC.mBindFlags = BIND_SHADER_RESOURCE;
		DESC.mMipLevels = 1;

		HDREnv = GetAssetManager().Import("@CommonAssets@/Textures/HDR/newport_loft.hdr", (DESC, Graphics::TextureUsageType::Unknown));

		Rendering::ImageBasedLightingDesc desc;
		IBL.Initialize(desc);

		HDR_Cube = IBL.EquirectangularToCubemap(&HDREnv);
		EnvCapture = IBL.PrecomputePBRCapture(&HDR_Cube);
		IBL.SetEnvironmentCapture(&EnvCapture);
	}

	void InitRenderer()
	{
		InitIBL();

		Renderer->AddRenderPass(&GeoPass);
		Renderer->AddRenderPass(&DefferedPass);
		Renderer->AddRenderPass(&PostFXPass);

		InitShaders();

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);	
		PostFXPass.Bake({ _Width_, _Height_,Rendering::RenderingEngine::GetInstance().GetFinalRT().GetDesc() });

		PostFXPass.SetPostProcessingEffect(Utilities::Hash("HDR"), false);
		PostFXPass.SetPostProcessingEffect(Utilities::Hash("GAMMACORRECTION"), false);
		PostFXPass.SetPostProcessingEffect(Utilities::Hash("BLOOM"), false);

		Skybox.Initialize(&HDR_Cube);
		//Renderer->GetBackground().SetSkybox(&Skybox);
	}

	void Load()
	{
		SampleBase::Load();

		EController.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Spot);

		SetupEntities();

		//Rendering::ShadowPassBakingDesc spdesc;

		//spdesc.MAX_OMNIDIR_CASTERS = 1;
		//spdesc.MAX_DIR_CASTERS = 0;
		//ShadowPass.Bake(spdesc);

		//Systems::LightingSystemDesc desc;
		//desc.ShadowPass = &ShadowPass;

		//mLightingSystem = GetScene().GetSystemManager().Add<Systems::LightingSystem>(desc);
		//mLightingSystem->Bake();

		InitRenderer();
	//	Renderer->AddRenderPass(&ShadowPass);

		SetupAssets();

		//Renderer->GetBackground().SetSkybox(&Skybox);
		Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
	}

	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		EController.GetComponent<Components::LightComponent>().SetDirection(GetScene().GetMainCamera()->GetFrontView());

		GetScene().Update(dt);
		{
			using namespace Graphics;
			ImGui::Begin("Sample3: Sponza Rendering");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			//Change Rendering Pipeline
			//if (e == 0)
			//	Renderer->SetActiveRenderingPipeline(WireFrameRPPipeline.GetID());
			//else if (e == 1)
			//	Renderer->SetActiveRenderingPipeline(DiffuseRPPipeline.GetID());
			//else if (e == 2)
			//	Renderer->SetActiveRenderingPipeline(BlinnPhongPipeline.GetID());
			//else if (e == 3)
			//	Renderer->SetActiveRenderingPipeline(PBRPipeline.GetID());
			//else if (e == 4)
			//	Renderer->SetActiveRenderingPipeline(IBLPipeline.GetID());

			//else if (e == 5)
			//	Renderer->SetActiveRenderingPipeline(Deffered_BlinnPhongPipeline.GetID());
			//else if (e == 6)
			//	Renderer->SetActiveRenderingPipeline(Deffered_PBRPipeline.GetID());
			//else if (e == 7)
			//	Renderer->SetActiveRenderingPipeline(Deffered_IBLPipeline.GetID());



			ImGui::Text("Material");

			if (ImGui::Button("PBR"))
			{
				ESponza.GetComponent<Components::MeshComponent>().SetMaterial(&SponzaPBRMaterial);
			}
			if (ImGui::Button("BlinnPhong"))
			{
				ESponza.GetComponent<Components::MeshComponent>().SetMaterial(&SponzaBlinnPhongMaterial);
			}

			ImGui::ColorEdit3("Camera ClearColor", (float*)&GetScene().GetMainCamera()->mRTClearColor);


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

			//ImGui::Checkbox("Visualize Pointlights", &Renderer->VisualizePointLightsPositions);

			//ImGui::Checkbox("Render Skybox", &GetScene().GetMainCamera()->RenderSkybox);

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