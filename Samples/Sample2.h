#pragma once
#include "Common.h"

void EntityView(entt::entity& entity, entt::registry& reg, Components::EntityInfoComponent& Einfo);
void EntityExplorer(ECS::Scene* scene);

class Sample2 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;

	Managers::CameraManager SceneCameraManager;

	//Assets::Mesh* SponzaAsset;
	//Assets::Material* SponzaMaterial;

	//Assets::Mesh* CerberusAsset;
	//Assets::Material* CerberusMaterial;

	Assets::Material SphereMaterial;
	Assets::Material PlaneMaterial;

	Components::CameraComponent* Camera;

	Graphics::PBR PBR;
	Graphics::BlinnPhong BlinnPhong;
	Graphics::DiffuseOnly DiffuseRP;
	Graphics::WireFrame WireFrameRP;

	//ECS
	ECS::Scene Scene;
	//ECS::Entity ESponza;
	//ECS::Entity ECerberus;
	ECS::Entity EPlane;

	ECS::Entity ECamera;
	ECS::Entity ELights;

	Assets::Mesh gPlane;

	std::vector<ECS::Entity> boxes;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;
public:
	Sample2()
	{

	}
	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;

		//Load Models
		//std::tie(SponzaAsset, SponzaMaterial) = mAssetManager->Import("Assets/Common/Models/CrytekSponza/sponza.obj", ModelDesc);
		//std::tie(CerberusAsset, CerberusMaterial) = mAssetManager->Import("Assets/Common/Models/Cerberus/Cerberus.FBX", ModelDesc);

		//Load some textures manually
		Importers::ImageLoadingDesc desc;
		desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet PBRSphereSet;
		PBRSphereSet.mData.push_back({ 0, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/albedo.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Diffuse) });
		PBRSphereSet.mData.push_back({ 1, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/metallic.png", Importers::ImageLoadingDesc(),Graphics::TextureUsageType::Specular) });
		PBRSphereSet.mData.push_back({ 2, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/normal.png",Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Normal) });
		PBRSphereSet.mData.push_back({ 3, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::Roughness) });
		PBRSphereSet.mData.push_back({ 4, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/roughness.png", Importers::ImageLoadingDesc(), Graphics::TextureUsageType::AO) });

		SphereMaterial.mPixelShaderTextures.push_back(PBRSphereSet);
		Renderer->CreateMaterialForAllPipelines(&SphereMaterial);
		//Renderer->CreateMaterialForAllPipelines(SponzaMaterial);
		//Renderer->CreateMaterialForAllPipelines(CerberusMaterial);

		//ESponza.Assign<Components::MeshComponent>(SponzaAsset, SponzaMaterial);
		//ECerberus.Assign<Components::MeshComponent>(CerberusAsset, CerberusMaterial);

		Assets::TextureSet PlaneSet;
		PlaneSet.mData.push_back({ 0, Managers::AssetManager::DefaultGreyTex });

		PlaneMaterial.mPixelShaderTextures.push_back(PlaneSet);

		Renderer->CreateMaterialForAllPipelines(&PlaneMaterial);

		PBRSphereSet.mData.clear();
	}
	void SetupEntities()
	{
		//Create Entities
		//ESponza = Scene.CreateEntity();
		ELights = Scene.CreateEntity("Lights");
		ECamera = Scene.CreateEntity("Controller");
		//ECerberus = Scene.CreateEntity();
		EPlane = Scene.CreateEntity("Plane");

		//Assign Components
	
		ELights.AddComponent<Components::DirLightComponent>();
		ELights.AddComponent<Components::PointLightComponent>();
		ECamera.AddComponent<Components::SpotLightComponent>();
		Camera = &ECamera.AddComponent<Components::CameraComponent>();

		Camera->Initialize(Math::perspective(Math::radians(45.0f), Core::Engine::GetInstance()->GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(Camera);

		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		ELights.GetComponent<Components::PointLightComponent>()->SetPosition(Math::Vector3(0.0f, 0.0f, 10.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.0f);

	}
	void InitRenderer()
	{
		Renderer = Scene.GetSystemManager().Add<Systems::RenderSystem>(&SceneCameraManager);
		Systems::PhysXSystemDesc sceneDesc;
		sceneDesc.mGravity = Math::Vector3(0.0f, -7.0f, 0.0f);
		mPhysXSystem = Scene.GetSystemManager().Add<Systems::PhysXSystem>(sceneDesc);

		//Scene.Systems.Configure();
		Renderer->AddRenderingPipeline(&PBR);
		Renderer->AddRenderingPipeline(&BlinnPhong);
		Renderer->AddRenderingPipeline(&DiffuseRP);
		Renderer->AddRenderingPipeline(&WireFrameRP);

		Renderer->Bake();
	}

	void Load()
	{
		mAssetManager->Initialize();
		SetupEntities();

		InitRenderer();

		SetupAssets();

		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
		Math::Matrix4 model;
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				model = Math::Matrix4(1.0);
				model = Math::translate(model, Math::Vector3(
					(float)(col - (nrColumns / 2)) * spacing ,
					(float)(row - (nrRows / 2)) * spacing ,
					0.0f
				));
				model = Math::scale(model, Math::Vector3(2.0f));
				model = Math::translate(model, Math::Vector3(1.0f,5.0f,1.0f));
				Scene.GetFactory().CreateSphere(&SphereMaterial, Math::translate(model, Math::Vector3(1.0f, 5.0f, 1.0f)));
				boxes.push_back(Scene.GetFactory().CreateBox(&SphereMaterial, model));
			}
		}
		for (auto it : boxes)
		{
			it.GetComponent<Components::RigidBodyComponent>()->isKinematic = true;
		}
		Math::Matrix4 TSponza(1.0f);
		TSponza = Math::scale(TSponza, Math::Vector3(0.05f));
		//ESponza.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TSponza);

		Math::Matrix4 TCerberus(1.0f);
		TCerberus = Math::scale(TCerberus, Math::Vector3(0.05f));
		//ECerberus.GetComponent<Components::EntityInfoComponent>()->mTransform.SetTransform(TCerberus);

		Components::CameraBakingOptions Desc;
		Desc.RTWidth = _Width_;
		Desc.RTHeight = _Height_;
		Desc.Disable_Bloom_Varient = true;
		Camera->Bake(Desc);

		Camera->RTClearColor = Graphics::Color(0.15f, 0.15f, 0.15f, 1.0f);

		Assets::Mesh::CreatePlane(&gPlane, 100.0f, 100.0f);

		//Assign Components
		EPlane.AddComponent<Components::MeshComponent>(&gPlane, &PlaneMaterial);
		Components::ColliderDesc desc;
		desc.mShape = Components::ColliderShape::Plane;
		auto geo =	PhysX::PlaneGeometry(ECS::Transform());
		desc.mGeo = &geo;
		EPlane.AddComponent<Components::ColliderComponent>(desc);

		mPhysXSystem->AddActor(EPlane);

		Camera->GammaCorrection = true;
		Camera->HDR = true;
		Camera->MovementSpeed = 15;

		Core::Engine::GetInstance()->GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
	}
	void OnMouseMovement(int xpos_a, int ypos_a) override
	{
		if (!isMouseDisabled)
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
			float yoffset = lastY - ypos;

			lastX = xpos;
			lastY = ypos;

			Camera->ProcessEye(xoffset, yoffset);
		}
	}
	void OnWindowResize(int width, int height) override
	{
		//Graphics::Context::GetSwapChain()->Resize(width, height);
		//Camera->SetProjectionMatrix(Math::perspective(Math::radians(45.0f), 0.0f, 0.1f, 100.0f));
		//Camera->ResizeRenderTarget(width, height);
	}

	void Update(float deltatime) override
	{
		//Movement
		if (Core::Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_FORWARD, deltatime);
		if (Core::Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_LEFT, deltatime);
		if (Core::Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_BACKWARD, deltatime);
		if (Core::Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_RIGHT, deltatime);

		//Change Mouse Mode
		if (Core::Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = true;
			Core::Engine::GetInstance()->GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);
		}
		if (Core::Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_M) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = false;
			Core::Engine::GetInstance()->GetMainWindow()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
		}

		Camera->Update();
		SceneCameraManager.UpdateBuffer();

		Camera->UpdatePSO();
	}
	bool iskinematic = false;

	void Render(float dt) override
	{
		Scene.Update(dt);

		ECamera.GetComponent<Components::SpotLightComponent>()->SetPosition(Camera->GetPosition());
		ECamera.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera->GetFrontView());


		{
			using namespace Graphics;
			ImGui::Begin("Sample2: Advanced Rendering");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");
			if (ImGui::TreeNode("Rendering"))
			{
				ImGui::Text("Active Rendering Pipeline:");
				static int e = 0;
				ImGui::RadioButton("PBR", &e, 0);
				ImGui::RadioButton("BlinnPhong", &e, 1);
				ImGui::RadioButton("DiffuseOnly", &e, 2);
				ImGui::RadioButton("WireFrame", &e, 3);

				//Change Rendering Pipeline
				if (e == 0)
					Renderer->SetActiveRenderingPipeline(PBR.GetID());
				else if (e == 1)
					Renderer->SetActiveRenderingPipeline(BlinnPhong.GetID());
				else if (e == 2)
					Renderer->SetActiveRenderingPipeline(DiffuseRP.GetID());
				else if (e == 3)
					Renderer->SetActiveRenderingPipeline(WireFrameRP.GetID());

				ImGui::Checkbox("Visualize Pointlights", &Renderer->VisualizePointLightsPositions);

				ImGui::TreePop();
			}



			PhysX::RaycastHit hit;
			if (ImGui::TreeNode("Raycast Info"))
			{
				if (Core::Engine::GetInstance()->GetMainWindow()->GetKeyStatus(Core::Input::KeyboardKey::KEY_F) == Core::Input::KeyboardKeyStatus::Pressed)
				{

					if (mPhysXSystem->Raycast(Camera->GetPosition(), Camera->GetFrontView(), 100.f, hit))
					{
						auto entity = hit.HitEntity;

						ImGui::Text((char*)Scene.GetRegistry().try_get<Components::EntityInfoComponent>(entity.entity)->mName.c_str());
					}
					else
					{
						ImGui::Text("No hit");
					}
				}
				else
					ImGui::Text("Press F");

				ImGui::TreePop();
			}

			//if (ImGui::TreeNode("Physics"))
			//{
			//	ImGui::Checkbox("iskinematic", &iskinematic);
			//	if (ImGui::Button("Update Physics"))
			//	{
			//	/*	for (auto it : boxes)
			//		{
			//			it.GetComponent<Components::RigidBodyComponent>()->isKinematic = true;
			//		}*/
			//	}
			//	ImGui::TreePop();
			//}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Core::Engine::GetInstance()->EndGame();
			}

			ImGui::End();
			EntityExplorer(&Scene);

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}

	void Shutdown() override
	{
		mAssetManager->FlushContainers();
	}
};

void EntityExplorer(ECS::Scene* scene)
{
	//ImGui::ShowStackToolWindow();
	ImGui::Begin("Entity Explorer");
	if (ImGui::TreeNode(scene->GetName().c_str()))
	{
		auto view = scene->GetRegistry().view<Components::EntityInfoComponent>();

		for (auto entity : view)
		{
			auto& Einfo = view.get<Components::EntityInfoComponent>(entity);

			auto index = (Uint32)entity;

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			static int selectedindex = 0;

			if (selectedindex == index)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
				EntityView(entity,scene->GetRegistry(), Einfo);
			}

			ImGui::PushID(index);


			ImGui::TreeNodeEx(Einfo.mName.c_str(), node_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				selectedindex = index;
			}

			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	ImGui::End();



}

void EntityView(entt::entity& entity, entt::registry& reg, Components::EntityInfoComponent& Einfo)
{
	auto index = (Uint32)entity;

	using namespace Graphics;
	ImGui::Begin("Entity View");
	{
		ImGui::PushID(index);           // Push i to the id tack

		ImGui::InputText("Name", (char*)Einfo.mName.c_str(), Einfo.mName.capacity() + 1);
		if (ImGui::CollapsingHeader("Transform"))
		{
			//auto* transform = &Einfo.mTransform;

			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("Transformtab", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Local"))
				{
					/*ImGui::DragFloat3("Position", (float*)&transform->GetLocalPosition());
					ImGui::DragFloat3("Rotation", (float*)&transform->GetLocalRotation());
					ImGui::DragFloat3("Scale", (float*)&transform->GetLocalScale());*/
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("World"))
				{
					//Math::Vector3 pos = transform->GetWorldPosition();
					//ImGui::DragFloat3("Position", (float*)&pos);
					//auto test = Einfo.Get();
					//test->mTransform.SetTransform(Math::translate(test->mTransform.GetTransform(), pos));

					/*ImGui::DragFloat3("Rotation", (float*)&transform->GetWorldRotation());
					ImGui::DragFloat3("Scale", (float*)&transform->GetWorldScale());*/
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}

		//Collider
		{
			Components::ColliderComponent* colliderbody = reg.try_get< Components::ColliderComponent>(entity);
			if (colliderbody)
			{
				if (ImGui::CollapsingHeader("Collider"))
				{
					//ImGui::Checkbox("iskinematic", &rigidbody->isKinematic);


				}
			}
		}
		
		//Rigid Body
		{

			Components::RigidBodyComponent* rigidbody = reg.try_get< Components::RigidBodyComponent>(entity);
			if (rigidbody)
			{
				if (ImGui::CollapsingHeader("Rigidbody"))
				{
					ImGui::Checkbox("iskinematic", &rigidbody->isKinematic);


				}
			}
		}
		//Camera
		{
			Components::CameraComponent* camera = reg.try_get< Components::CameraComponent>(entity);
			if (camera)
			{
				if (ImGui::CollapsingHeader("Camera"))
				{
					ImGui::ColorEdit3("RenderTarget ClearColor", (float*)&camera->RTClearColor);
					ImGui::Checkbox("HDR", &camera->HDR);
					ImGui::Checkbox("GammaCorrection", &camera->GammaCorrection);

					ImGui::DragFloat("Movement Speed", &camera->MovementSpeed);
					ImGui::DragFloat("Mouse Sensitivity", &camera->MouseSensitivity);
					ImGui::DragFloat("Zoom", &camera->Zoom);


				}
			}
		}

		//DirLight
		{
			Components::DirLightComponent* light = reg.try_get< Components::DirLightComponent>(entity);
			if (light)
			{
				if (ImGui::CollapsingHeader("Directional Light"))
				{
					Graphics::Color oldcolor = light->GetColor();
					ImVec4 Lightcolor = ImVec4(oldcolor.r, oldcolor.g, oldcolor.b, 1.00f);
					if (ImGui::ColorEdit3("Color", (float*)&Lightcolor))
					{
						light->SetColor(Graphics::Color(Lightcolor.x, Lightcolor.y, Lightcolor.z, Lightcolor.w));
					}

					ImVec4 lighdir = ImVec4(light->GetDirection().x, light->GetDirection().y, light->GetDirection().z, 1.00f);
					if (ImGui::DragFloat3("Direction", (float*)&lighdir))
					{
						light->SetDirection(Math::Vector3(lighdir.x, lighdir.y, lighdir.z));
					}


				}
			}
		}

		//PointLight
		{
			Components::PointLightComponent* light = reg.try_get< Components::PointLightComponent>(entity);
			if (light)
			{
				if (ImGui::CollapsingHeader("Point Light"))
				{
					ImVec4 lighpos = ImVec4(light->GetPosition().x, light->GetPosition().y, light->GetPosition().z, 1.00f);
					if (ImGui::DragFloat3("Position", (float*)&lighpos))
					{
						light->SetPosition(Math::Vector3(lighpos.x, lighpos.y, lighpos.z));
					}

					Graphics::Color oldcolor = light->GetColor();
					ImVec4 Lightcolor = ImVec4(oldcolor.r, oldcolor.g, oldcolor.b, 1.00f);
					if (ImGui::ColorEdit3("Color", (float*)&Lightcolor))
					{
						light->SetColor(Graphics::Color(Lightcolor.x, Lightcolor.y, Lightcolor.z, Lightcolor.w));
					}

					ImVec4 att = ImVec4(light->GetAttenuation().x, light->GetAttenuation().y, light->GetAttenuation().z, 1.00f);
					if (ImGui::DragFloat3("Attenuation", (float*)&att))
					{
						light->SetAttenuation(Math::Vector3(att.x, att.y, att.z));
					}

					float f = light->GetIntensity();
					if (ImGui::SliderFloat("Intensity", &f, 0.0f, 100.0f, "%.4f", ImGuiSliderFlags_None))
					{
						light->SetIntensity(f);
					}

				}
			}
		}

		//SpotLight
		{
			Components::SpotLightComponent* light = reg.try_get< Components::SpotLightComponent>(entity);
			if (light)
			{
				if (ImGui::CollapsingHeader("Spot Light"))
				{
					ImVec4 lighpos = ImVec4(light->GetPosition().x, light->GetPosition().y, light->GetPosition().z, 1.00f);
					if (ImGui::DragFloat3("Position", (float*)&lighpos))
					{
						light->SetPosition(Math::Vector3(lighpos.x, lighpos.y, lighpos.z));
					}

					ImVec4 lighdir = ImVec4(light->GetDirection().x, light->GetDirection().y, light->GetDirection().z, 1.00f);
					if (ImGui::DragFloat3("Direction", (float*)&lighdir))
					{
						light->SetDirection(Math::Vector3(lighdir.x, lighdir.y, lighdir.z));
					}

					ImVec2 cone = ImVec2(light->GetSpotlightCone().x, light->GetSpotlightCone().y);
					if (ImGui::DragFloat2("Spotlight Cone", (float*)&cone))
					{
						light->SetSpotlightCone(Math::Vector2(cone.x, cone.y));
					}

					Graphics::Color oldcolor = light->GetColor();
					ImVec4 Lightcolor = ImVec4(oldcolor.r, oldcolor.g, oldcolor.b, 1.00f);
					if (ImGui::ColorEdit3("Color", (float*)&Lightcolor))
					{
						light->SetColor(Graphics::Color(Lightcolor.x, Lightcolor.y, Lightcolor.z, Lightcolor.w));
					}

					ImVec4 att = ImVec4(light->GetAttenuation().x, light->GetAttenuation().y, light->GetAttenuation().z, 1.00f);
					if (ImGui::DragFloat3("Attenuation", (float*)&att))
					{
						light->SetAttenuation(Math::Vector3(att.x, att.y, att.z));
					}

					float f = light->GetIntensity();
					if (ImGui::SliderFloat("Intensity", &f, 0.0f, 100.0f, "%.4f", ImGuiSliderFlags_None))
					{
						light->SetIntensity(f);
					}

				}
			}
		}

		//Mesh
		{
			Components::MeshComponent* meshcomponent = reg.try_get< Components::MeshComponent>(entity);
			if (meshcomponent) {

				if (ImGui::CollapsingHeader("Mesh"))
				{
					ImGui::Checkbox("Render", &meshcomponent->mRender);
					ImGui::Checkbox("Multi Render", &meshcomponent->mMultiRender);

					if (meshcomponent->mMesh != nullptr)
					{
						if (meshcomponent->mMesh->GetStringName() != std::string(""))
						{
							ImGui::Text(meshcomponent->mMesh->GetStringName().c_str());
						}
						else {
							ImGui::Text("Unnamed Mesh");
						}
					}
					if (meshcomponent->mMaterial != nullptr)
					{
						if (meshcomponent->mMaterial->GetStringName() != std::string(""))
						{
							ImGui::Text(meshcomponent->mMaterial->GetStringName().c_str());
						}
						else {
							ImGui::Text("Unnamed Material");
						}
					}

				}
			}
		}

		ImGui::PopID();
	}
	ImGui::End();
}
