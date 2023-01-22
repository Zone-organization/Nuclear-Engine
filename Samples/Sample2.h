#pragma once
#include "SampleBase.h"

class Sample2 : public SampleBase
{
	Assets::Mesh* ShaderBall;
	
	Assets::Material RustedIron;
	Assets::Material Plastic;
	Assets::Material Grass;
	Assets::Material Gold;
	Assets::Material Wall;

	Assets::Shader* PBR;

	Rendering::GeometryPass GeoPass;
	Rendering::PostProcessingPass PostFXPass;
	Rendering::DefferedPass DefferedPass;

	//IBL Settings
	Rendering::ImageBasedLighting IBL;
	Rendering::PBRCapture EnvCapture;
	Assets::MaterialTexture HDREnv;
	Assets::Texture HDR_Cube;

	Rendering::Skybox Skybox;

	ECS::Entity EShaderBall;
	ECS::Entity ESphere;

	ECS::Entity ELights;

	std::vector<ECS::Entity> boxes;
public:
	Sample2()
	{

	}

	void LoadPBRMaterials()
	{
		//Initialize Materials
		Assets::MaterialTextureSet PBRRustedIron;
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/albedo.png") , Assets::TextureUsageType::Diffuse});
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/metallic.png") , Assets::TextureUsageType::Specular});
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/normal.png") , Assets::TextureUsageType::Normal});
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/roughness.png") ,  Assets::TextureUsageType::Roughness});
		PBRRustedIron.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/RustedIron/ao.png") ,  Assets::TextureUsageType::AO});
		
		Assets::MaterialCreationInfo RustedIron_D;
		RustedIron_D.mTextures.push_back(PBRRustedIron);
		RustedIron.SetName("RustedIron Material");

		Assets::MaterialTextureSet PBRPlastic;
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/albedo.png") , Assets::TextureUsageType::Diffuse});
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/metallic.png") , Assets::TextureUsageType::Specular});
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/normal.png") , Assets::TextureUsageType::Normal});
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/roughness.png") ,  Assets::TextureUsageType::Roughness});
		PBRPlastic.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/plastic/ao.png") ,  Assets::TextureUsageType::AO});
		
		Assets::MaterialCreationInfo Plastic_D;
		Plastic_D.mTextures.push_back(PBRPlastic);
		Plastic.SetName("Plastic Material");

		Assets::MaterialTextureSet PBRGrass;
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/albedo.png") , Assets::TextureUsageType::Diffuse});
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/metallic.png") , Assets::TextureUsageType::Specular});
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/normal.png") , Assets::TextureUsageType::Normal});
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/roughness.png") ,  Assets::TextureUsageType::Roughness});
		PBRGrass.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/grass/ao.png") ,  Assets::TextureUsageType::AO});

		Assets::MaterialCreationInfo Grass_D;
		Grass_D.mTextures.push_back(PBRGrass);
		Grass.SetName("Grass Material");

		Assets::MaterialTextureSet PBRWall;
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/albedo.png") , Assets::TextureUsageType::Diffuse});
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/metallic.png") , Assets::TextureUsageType::Specular});
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/normal.png") , Assets::TextureUsageType::Normal});
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/roughness.png") ,  Assets::TextureUsageType::Roughness});
		PBRWall.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/wall/ao.png") ,  Assets::TextureUsageType::AO});

		Assets::MaterialCreationInfo Wall_D;
		Wall_D.mTextures.push_back(PBRWall);
		Wall.SetName("Wall Material");

		Assets::MaterialTextureSet PBRGold;
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/albedo.png") , Assets::TextureUsageType::Diffuse});
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/metallic.png") , Assets::TextureUsageType::Specular});
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/normal.png") , Assets::TextureUsageType::Normal});
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/roughness.png") ,  Assets::TextureUsageType::Roughness});
		PBRGold.mData.push_back({ GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/PBR/gold/ao.png") ,  Assets::TextureUsageType::AO});

		Assets::MaterialCreationInfo Gold_D;
		Gold_D.mTextures.push_back(PBRGold);
		Gold.SetName("Gold Material");

		RustedIron.Create(RustedIron_D, PBR);
		Plastic.Create(Plastic_D, PBR);
		Grass.Create(Grass_D, PBR);
		Wall.Create(Wall_D, PBR);
		Gold.Create(Gold_D, PBR);
	}

	void SetupAssets()
	{
		Assets::ModelImportingDesc ModelDesc;
		ModelDesc.LoadMaterialInfo = false;
		ModelDesc.LoadAnimation = false;

		//Load Models
		auto shaderballasset = GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/shaderball/shaderball.obj", ModelDesc);
		ShaderBall = shaderballasset->pMesh;
		LoadPBRMaterials();
	}
	void SetupEntities()
	{
		//Create Entities
		ECS::Transform shaderballT;
		shaderballT.SetScale(0.5f);

		EShaderBall = GetScene().CreateEntity("ShaderBall" , shaderballT);
		ELights = GetScene().CreateEntity("Lights");
		EController = GetScene().CreateEntity("Controller");

		//Assign Components
		ELights.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);

		ELights.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Math::Vector3(0.0f, 5.0f, 10.0f));
		ELights.GetComponent<Components::LightComponent>().SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::LightComponent>().SetIntensity(10.0f);

	}
	void InitIBL()
	{
		//IBL
		Assets::TextureImportingDesc desc;
		desc.mType = Diligent::RESOURCE_DIM_TEX_2D;
		desc.mUsage = Diligent::USAGE_IMMUTABLE;
		desc.mBindFlags = Diligent::BIND_SHADER_RESOURCE;
		desc.mMipLevels = 1;
		desc.mCommonOptions.mAsyncImport = false;
		HDREnv.pTexture = GetAssetManager().Import<Assets::Texture>("@CommonAssets@/Textures/HDR/newport_loft.hdr", desc);

		Rendering::ImageBasedLightingDesc ibldesc;
		IBL.Initialize(ibldesc);

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

		Assets::ShaderImportingDesc desc;
		desc.mType = Graphics::ShaderType::_3DRendering;
		PBR = GetAssetManager().Import<Assets::Shader>("@NuclearAssets@/Shaders/PBR/PBR.NuclearShader", desc);

		Renderer->RegisterShader(PBR);
		Renderer->SetIBLContext(&IBL);

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);

		PostFXPass.Bake({ _Width_, _Height_,Rendering::RenderingEngine::GetInstance().GetFinalRT().GetDesc() });
	}

	void Load() override
	{
		SampleBase::Load();
		SetupEntities();

		InitRenderer();
		ECS::Transform sphere;
		sphere.SetScale(Math::Vector3(5.0f));
		ESphere = GetScene().CreateSphere(&RustedIron, sphere);

		SetupAssets();

		EShaderBall.AddComponent<Components::MeshComponent>(ShaderBall, &RustedIron);
		EShaderBall.GetComponent<Components::MeshComponent>().SetEnableRendering(true);
		ESphere.GetComponent<Components::MeshComponent>().SetEnableRendering(false);

		GetScene().GetMainCamera()->mRTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);

		Skybox.Initialize(&HDR_Cube);
		PostFXPass.GetBackground().SetSkybox(&Skybox);

		Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
	}

	bool iskinematic = false;
	bool isDeffered = true;
	void Render(float dt) override
	{
		GetScene().Update(dt);
		{

			using namespace Graphics;
			ImGui::Begin("Sample2: Advanced Rendering");

			static float rotationspeed = 0.0f;
			static Math::Vector3 RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
			static ECS::Entity activeentity = EShaderBall;
			ImGui::Text("Model");

			static int f = 0;
			if (ImGui::RadioButton("ShaderBall", &f, 0))
			{
				activeentity = EShaderBall;
				EShaderBall.GetComponent<Components::MeshComponent>().SetEnableRendering(true);
				ESphere.GetComponent<Components::MeshComponent>().SetEnableRendering(false);
			}
			if (ImGui::RadioButton("Sphere", &f, 1))
			{
				activeentity = ESphere;
				EShaderBall.GetComponent<Components::MeshComponent>().SetEnableRendering(false);
				ESphere.GetComponent<Components::MeshComponent>().SetEnableRendering(true);
			}


			ImGui::Text("Material");

			if (ImGui::Checkbox("Use Deffered Pipeline", &isDeffered))
			{
				ESphere.GetComponent<Components::MeshComponent>().SetVariantSwitch(Utilities::Hash("NE_DEFFERED"), isDeffered);
				EShaderBall.GetComponent<Components::MeshComponent>().SetVariantSwitch(Utilities::Hash("NE_DEFFERED"), isDeffered);
			}

			static bool IBL_ = false;

			if (ImGui::Checkbox("IBL", &IBL_))
			{
				ESphere.GetComponent<Components::MeshComponent>().SetVariantSwitch(Utilities::Hash("IBL_ENABLED"), IBL_);
				EShaderBall.GetComponent<Components::MeshComponent>().SetVariantSwitch(Utilities::Hash("IBL_ENABLED"), IBL_);
			}
			if (ImGui::Button("Rusted Iron"))
			{
				activeentity.GetComponent<Components::MeshComponent>().SetMaterial(&RustedIron);
			}
			if (ImGui::Button("Wall"))
			{
				activeentity.GetComponent<Components::MeshComponent>().SetMaterial(&Wall);
			}
			if (ImGui::Button("Grass"))
			{
				activeentity.GetComponent<Components::MeshComponent>().SetMaterial(&Grass);
			}
			if (ImGui::Button("Gold"))
			{
				activeentity.GetComponent<Components::MeshComponent>().SetMaterial(&Gold);
			}
			if (ImGui::Button("Plastic"))
			{
				activeentity.GetComponent<Components::MeshComponent>().SetMaterial(&Plastic);
			}


			ImGui::SliderFloat("Rotation Speed", &rotationspeed, 0.0f, 2.0f);

			ImGui::SliderFloat3("Rotation Axis", (float*)&RotationAxis, 0.0f, 1.0f);

			float rotationAngle = LastFrame / 5.0f * rotationspeed;
			activeentity.GetComponent<Components::EntityInfoComponent>().mTransform.SetRotation(RotationAxis, rotationAngle);

			ImGui::Separator();
			ImGui::Text("PostFX Pipeline");

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


			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Checkbox("Visualize RenderTargets", &DebugSystem->ShowRegisteredRenderTargets);

			ImGui::Checkbox("Visualize LightSources", &DebugSystem->RenderLightSources);

			ImGui::ColorEdit3("Camera ClearColor", (float*)&GetScene().GetMainCamera()->mRTClearColor);

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
};