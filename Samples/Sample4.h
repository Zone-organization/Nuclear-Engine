#pragma once
#include "SampleBase.h"

class Sample4: public SampleBase
{
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;

	Assets::Material* RustedIron;
	Assets::Material* Plastic;

	Assets::Shader* PBR;
	Rendering::ForwardRenderingPath ForwardRP;

	Rendering::GeometryPass GeoPass;
//	Rendering::PostProcessingPass PostFXPass;

	std::vector<ECS::Entity> boxes;

	ECS::Entity ECamera2;
	Uint32 CameraRTWidth = 800, CameraRTHeight = 600;
public:
	Sample4()
	{

	}
	void SetupAssets()
	{
		GetAssetManager().LoadFolder("@Assets@/Textures/PBR/RustedIron/");
		GetAssetManager().LoadFolder("@Assets@/Textures/PBR/Plastic/");

		RustedIron = GetAssetManager().Load<Assets::Material>("@Assets@/Materials/PBR/RustedIron.NEMaterial");
		Plastic = GetAssetManager().Load<Assets::Material>("@Assets@/Materials/PBR/Plastic.NEMaterial");
	}
	void SetupEntities()
	{
		auto EDirLight = GetScene().CreateEntity("DirLight");
		auto& dircomp = EDirLight.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Directional);
		dircomp.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dircomp.SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		auto ELights = GetScene().CreateEntity("PointLight1");
		auto& lightcomp = ELights.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);
		lightcomp.SetIntensity(10.0f);

		EController.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Spot);

		ELights.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Math::Vector3(0.0f, 5.0f, 10.0f));
	}
	void InitRenderer()
	{
		Systems::PhysXSystemDesc sceneDesc;
		sceneDesc.mGravity = Math::Vector3(0.0f, -7.0f, 0.0f);
		mPhysXSystem = GetScene().GetSystemManager().Add<Systems::PhysXSystem>(sceneDesc);

		Renderer->AddRenderPass(&GeoPass);
		//Renderer->AddRenderPass(&PostFXPass);

		PBR = GetAssetManager().Import<Assets::Shader>("@NuclearAssets@/Shaders/PBR/PBR.NuclearShader");

		Renderer->RegisterShader(PBR);

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);

		//PostFXPass.Bake({ _Width_, _Height_,Rendering::RenderingEngine::GetInstance().GetFinalRT().GetDesc(} });
	}
	void FillScene()
	{
		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				Math::Vector3 position(
					(float)(col - (nrColumns / 2)) * spacing,
					((float)(row - (nrRows / 2)) * spacing) + 10.0f,
					0.0f
				);


				ECS::Transform ESphere(position, Math::Vector3(2.0f));

				auto sphere = GetScene().CreateSphere(RustedIron, ESphere);
				position.z += 5.0f;

				//ECS::Transform EBox(position, Math::Vector3(1.0f));

				//boxes.push_back(GetScene().GetFactory().CreateBox(&SphereMaterial, EBox));
			}
		}

		GetScene().CreatePlane(Plastic);
		for (auto it : boxes)
		{
			it.GetComponent<Components::RigidBodyComponent>().isKinematic = true;
		}

	}
	void Load()
	{
		SampleBase::Load();

		SetupEntities();

		InitRenderer();

		SetupAssets();

		FillScene();

		//Main Camera RT
		GetScene().GetMainCamera()->mRTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);

		//Initialize Camera2
		{	
			//Create Entity
			ECS::Transform camtrans;
			camtrans.SetPosition({ 0.0f, 2.0f, 40.0f });
			ECamera2 = GetScene().CreateEntity("Camera2", camtrans);

			Components::CameraComponentDesc cameradesc;
			cameradesc.mProjection = Math::perspective(Math::radians(45.0f), static_cast<Float32>(CameraRTWidth / CameraRTHeight), 0.1f, 100.0f);

			Math::Vector2ui Camera2Dimensions(CameraRTWidth, CameraRTHeight);

			Graphics::RenderTargetDesc RTDesc, RTDepthDesc;
			RTDesc.mName = "Camera2RT";
			RTDesc.mDimensions = Camera2Dimensions;
			RTDesc.ColorTexFormat = Diligent::TEX_FORMAT_RGBA16_FLOAT;

			RTDepthDesc.mName = "Camera2DepthRT";
			RTDepthDesc.mDimensions = Camera2Dimensions;
			RTDepthDesc.DepthTexFormat = Graphics::Context::GetInstance().GetSwapChain()->GetDesc().DepthBufferFormat;
			
			cameradesc.mRTDesc = RTDesc;
			cameradesc.mDepthRTDesc = RTDepthDesc;
			
			//To enable rendering to it
			cameradesc.mIsActiveCamera = true;
			ECamera2.AddComponent<Components::CameraComponent>(cameradesc);
		} 

		Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
	}

	bool iskinematic = false;

	void Render(float dt) override
	{
		auto& camera2 = ECamera2.GetComponent<Components::CameraComponent>();
		camera2.SetPosition(ECamera2.GetComponent<Components::EntityInfoComponent>().mTransform.GetLocalPosition());

		GetScene().Update(dt);

		EController.GetComponent<Components::LightComponent>().SetDirection(GetScene().GetMainCamera()->GetFrontView());


		{
			using namespace Graphics;
			ImGui::Begin("Sample4: Multiple Cameras");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");


			ImGui::ColorEdit3("Main Camera ClearColor", (float*)&GetScene().GetMainCamera()->mRTClearColor);

			ImGui::ColorEdit3("Camera 2 ClearColor", (float*)&camera2.mRTClearColor);

			ImGui::Text("Camera2 RT");

			ImGui::Separator();

			ImGui::Image(camera2.GetRenderTarget().GetSRV(), { 800, 600 });
			ImGui::Separator();

		/*	ImGui::Text("PostFX Pipeline");

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
			}*/

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