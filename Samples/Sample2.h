#pragma once
#include "Common.h"

void EntityView(ECS::Entity& entity, ECS::ComponentHandle<Components::EntityInfoComponent>& Einfo);

void EntityExplorer(ECS::Scene* scene)
{
	ImGui::ShowStackToolWindow();

	ECS::Entity Entityview;
	ImGui::Begin("Entity Explorer");
	if (ImGui::TreeNode(scene->GetName().c_str()))
	{
		ECS::ComponentHandle<Components::EntityInfoComponent> Einfo;

		for (ECS::Entity entity : scene->Entities.entities_with_components(Einfo))
		{
			auto index = entity.id().index();
			ECS::ComponentHandle<Components::EntityInfoComponent> Einfo = entity.GetComponent<Components::EntityInfoComponent>();

			if (!Einfo.Valid()) 
				assert("Entity with no info");

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			static int selectedindex = 0;

			if (selectedindex == index)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
				EntityView(entity, Einfo);
			}

			ImGui::PushID(entity.id().id());


			ImGui::TreeNodeEx(Einfo.Get()->mName.c_str(), node_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				selectedindex = index;
			}

			ImGui::PopID();

			//Entity Info
			/*
			std::string Index = std::to_string(index);
			if (ImGui::TreeNode(std::string(Index + ": " + Einfo.Get()->mName).c_str()))
			{
				ImGui::PushID(index);           // Push i to the id tack

				if (ImGui::TreeNode("Transform"))
				{

					ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
					if (ImGui::BeginTabBar("Transformtab", tab_bar_flags))
					{
						if (ImGui::BeginTabItem("Local"))
						{
							ImGui::DragFloat3("Position", (float*)&Einfo->mTransform.GetLocalPosition());
							ImGui::DragFloat3("Rotation", (float*)&Einfo->mTransform.GetLocalRotation());
							ImGui::DragFloat3("Scale", (float*)&Einfo->mTransform.GetLocalScale());			
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("World"))
						{
							Math::Vector3 pos = Einfo->mTransform.GetWorldPosition();
							ImGui::DragFloat3("Position", (float*)&pos);
							//auto test = Einfo.Get();
							//test->mTransform.SetTransform(Math::translate(test->mTransform.GetTransform(), pos));

							ImGui::DragFloat3("Rotation", (float*)&Einfo->mTransform.GetWorldRotation());
							ImGui::DragFloat3("Scale", (float*)&Einfo->mTransform.GetWorldScale());
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}



					//Einfo->mTransform.SetPosition(pos);
					ImGui::TreePop();
				}
				ImGui::PopID();

				//Collider
				{
					ECS::ComponentHandle<Components::ColliderComponent> colliderbody = entity.GetComponent<Components::ColliderComponent>();
					if (colliderbody.Valid()) {

						ImGui::PushID(index);           // Push i to the id tack

						if (ImGui::TreeNode("Collider"))
						{
							//ImGui::Checkbox("iskinematic", &rigidbody->isKinematic);

							ImGui::TreePop();
						}
						ImGui::PopID();
						// Do stuff with position
					}
				}

				//Rigid Body
				{
					ECS::ComponentHandle<Components::RigidBodyComponent> rigidbody = entity.GetComponent<Components::RigidBodyComponent>();
					if (rigidbody.Valid()) {

						ImGui::PushID(index);           // Push i to the id tack

						if (ImGui::TreeNode("Rigidbody"))
						{
							ImGui::Checkbox("iskinematic", &rigidbody->isKinematic);

							ImGui::TreePop();
						}
						ImGui::PopID();
						// Do stuff with position
					}
				}
				//Camera
				{
					ECS::ComponentHandle<Components::CameraComponent> camera = entity.GetComponent<Components::CameraComponent>();
					if (camera.Valid()) {

						ImGui::PushID(index);           // Push i to the id tack

						if (ImGui::TreeNode("Camera"))
						{
							ImGui::ColorEdit3("RenderTarget ClearColor", (float*)&camera->RTClearColor);
							ImGui::Checkbox("HDR", &camera->HDR);
							ImGui::Checkbox("GammaCorrection", &camera->GammaCorrection);

							ImGui::DragFloat("Movement Speed", &camera->MovementSpeed);
							ImGui::DragFloat("Mouse Sensitivity", &camera->MouseSensitivity);
							ImGui::DragFloat("Zoom", &camera->Zoom);

							ImGui::TreePop();
						}
						ImGui::PopID();
						// Do stuff with position
					}
				}

				//DirLight
				{
					ECS::ComponentHandle<Components::DirLightComponent> light = entity.GetComponent<Components::DirLightComponent>();
					if (light.Valid()) {

						ImGui::PushID(index);           // Push i to the id tack

						if (ImGui::TreeNode("Directional Light"))
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

							ImGui::TreePop();
						}
						ImGui::PopID();
						// Do stuff with position
					}
				}

				//PointLight
				{
					ECS::ComponentHandle<Components::PointLightComponent> light = entity.GetComponent<Components::PointLightComponent>();
					if (light.Valid()) {

						ImGui::PushID(index);           // Push i to the id tack

						if (ImGui::TreeNode("Point Light"))
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
							if (ImGui::SliderFloat("Intensity", &f, 0.0f, 100.0f, "%.4f", 2.0f))
							{
								light->SetIntensity(f);
							}
							ImGui::TreePop();
						}
						ImGui::PopID();
						// Do stuff with position
					}
				}

				//SpotLight
				{
					ECS::ComponentHandle<Components::SpotLightComponent> light = entity.GetComponent<Components::SpotLightComponent>();
					if (light.Valid()) {

						ImGui::PushID(index);           // Push i to the id tack

						if (ImGui::TreeNode("Spot Light"))
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
							if (ImGui::SliderFloat("Intensity", &f, 0.0f, 100.0f, "%.4f", 2.0f))
							{
								light->SetIntensity(f);
							}
							ImGui::TreePop();
						}
						ImGui::PopID();
						// Do stuff with position
					}
				}

				//Mesh
				{
					ECS::ComponentHandle<Components::MeshComponent> meshcomponent = entity.GetComponent<Components::MeshComponent>();
					if (meshcomponent.Valid()) {

						ImGui::PushID(index);           // Push i to the id tack

						if (ImGui::TreeNode("Mesh"))
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
							ImGui::TreePop();
						}
						ImGui::PopID();
						// Do stuff with position
					}
				}

				ImGui::TreePop();

			}
		*/}
		ImGui::TreePop();
	}
	ImGui::End();



}

void EntityView(ECS::Entity& entity, ECS::ComponentHandle<Components::EntityInfoComponent>& Einfo)
{
	auto index = entity.id().index();

	using namespace Graphics;
	ImGui::Begin("Entity View");
	{
		ImGui::PushID(index);           // Push i to the id tack

		ImGui::InputText("Name", (char*)Einfo.Get()->mName.c_str(), Einfo.Get()->mName.capacity() + 1);

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("Transformtab", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Local"))
				{
					ImGui::DragFloat3("Position", (float*)&Einfo->mTransform.GetLocalPosition());
					ImGui::DragFloat3("Rotation", (float*)&Einfo->mTransform.GetLocalRotation());
					ImGui::DragFloat3("Scale", (float*)&Einfo->mTransform.GetLocalScale());
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("World"))
				{
					Math::Vector3 pos = Einfo->mTransform.GetWorldPosition();
					ImGui::DragFloat3("Position", (float*)&pos);
					//auto test = Einfo.Get();
					//test->mTransform.SetTransform(Math::translate(test->mTransform.GetTransform(), pos));

					ImGui::DragFloat3("Rotation", (float*)&Einfo->mTransform.GetWorldRotation());
					ImGui::DragFloat3("Scale", (float*)&Einfo->mTransform.GetWorldScale());
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}

		//Collider
		{
			ECS::ComponentHandle<Components::ColliderComponent> colliderbody = entity.GetComponent<Components::ColliderComponent>();
			if (colliderbody.Valid())
			{
				if (ImGui::CollapsingHeader("Collider"))
				{
					//ImGui::Checkbox("iskinematic", &rigidbody->isKinematic);


				}
			}
		}

		//Rigid Body
		{
			ECS::ComponentHandle<Components::RigidBodyComponent> rigidbody = entity.GetComponent<Components::RigidBodyComponent>();
			if (rigidbody.Valid())
			{
				if (ImGui::CollapsingHeader("Rigidbody"))
				{
					ImGui::Checkbox("iskinematic", &rigidbody->isKinematic);


				}
			}
		}
		//Camera
		{
			ECS::ComponentHandle<Components::CameraComponent> camera = entity.GetComponent<Components::CameraComponent>();
			if (camera.Valid())
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
			ECS::ComponentHandle<Components::DirLightComponent> light = entity.GetComponent<Components::DirLightComponent>();
			if (light.Valid())
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
			ECS::ComponentHandle<Components::PointLightComponent> light = entity.GetComponent<Components::PointLightComponent>();
			if (light.Valid())
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
			ECS::ComponentHandle<Components::SpotLightComponent> light = entity.GetComponent<Components::SpotLightComponent>();
			if (light.Valid())
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
			ECS::ComponentHandle<Components::MeshComponent> meshcomponent = entity.GetComponent<Components::MeshComponent>();
			if (meshcomponent.Valid()) {

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

class Sample2 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;

	Core::Input Input;
	Managers::CameraManager SceneCameraManager;

	//Assets::Mesh* SponzaAsset;
	//Assets::Material* SponzaMaterial;

	//Assets::Mesh* CerberusAsset;
	//Assets::Material* CerberusMaterial;

	Assets::Material SphereMaterial;
	Assets::Material PlaneMaterial;

	PhysX::PhysXMaterial BoxPhysXMat;

	ECS::ComponentHandle<Components::CameraComponent> Camera;

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

	std::vector<ECS::Entity> boxes;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

	void SetupAssets()
	{
		Importers::MeshLoadingDesc ModelDesc;

		//Load Models
		//std::tie(SponzaAsset, SponzaMaterial) = mAssetManager->Import("Assets/Common/Models/CrytekSponza/sponza.obj", ModelDesc);
		//std::tie(CerberusAsset, CerberusMaterial) = mAssetManager->Import("Assets/Common/Models/Cerberus/Cerberus.FBX", ModelDesc);

		//Load some textures manually
		Importers::TextureLoadingDesc desc;
		desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet PBRSphereSet;
		PBRSphereSet.mData.push_back({ 0, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/albedo.png", Assets::TextureUsageType::Diffuse) });
		PBRSphereSet.mData.push_back({ 1, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/metallic.png", Assets::TextureUsageType::Specular) });
		PBRSphereSet.mData.push_back({ 2, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/normal.png", Assets::TextureUsageType::Normal) });
		PBRSphereSet.mData.push_back({ 3, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/roughness.png", Assets::TextureUsageType::Roughness) });
		PBRSphereSet.mData.push_back({ 4, mAssetManager->Import("Assets/Common/Textures/PBR/RustedIron/ao.png", Assets::TextureUsageType::AO) });

		SphereMaterial.mPixelShaderTextures.push_back(PBRSphereSet);
		Renderer->CreateMaterialForAllPipelines(&SphereMaterial);
		//Renderer->CreateMaterialForAllPipelines(SponzaMaterial);
		//Renderer->CreateMaterialForAllPipelines(CerberusMaterial);

		//ESponza.Assign<Components::MeshComponent>(SponzaAsset, SponzaMaterial);
		//ECerberus.Assign<Components::MeshComponent>(CerberusAsset, CerberusMaterial);

		Assets::TextureSet PlaneSet;
		PlaneSet.mData.push_back({ 0, Assets::DefaultTextures::DefaultGreyTex });

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
	
		ELights.Assign<Components::DirLightComponent>();
		ELights.Assign<Components::PointLightComponent>();
		ECamera.Assign<Components::SpotLightComponent>();
		Camera = ECamera.Assign<Components::CameraComponent>();

		Camera->Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		SceneCameraManager.Initialize(Camera.Get());

		ELights.GetComponent<Components::DirLightComponent>()->SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		ELights.GetComponent<Components::DirLightComponent>()->SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		ELights.GetComponent<Components::PointLightComponent>()->SetPosition(Math::Vector3(0.0f, 0.0f, 10.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));
		ELights.GetComponent<Components::PointLightComponent>()->SetIntensity(10.0f);

	}
	void InitRenderer()
	{
		Renderer = Scene.Systems.Add<Systems::RenderSystem>(&SceneCameraManager);
		Systems::PhysXSystemDesc sceneDesc;
		sceneDesc.mGravity = Math::Vector3(0.0f, -7.0f, 0.0f);
		mPhysXSystem = Scene.Systems.Add<Systems::PhysXSystem>(&Scene, sceneDesc);

		Scene.Systems.Configure();
		Renderer->AddRenderingPipeline(&PBR);
		Renderer->AddRenderingPipeline(&BlinnPhong);
		Renderer->AddRenderingPipeline(&DiffuseRP);
		Renderer->AddRenderingPipeline(&WireFrameRP);

		Renderer->Bake(Scene.Entities);
	}

	void Load()
	{
		SetupEntities();

		InitRenderer();

		SetupAssets();
		Scene.Factory.InitializeDefaultPhysxMaterials();

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
				Scene.Factory.CreateSphere(&SphereMaterial, Math::translate(model, Math::Vector3(1.0f, 5.0f, 1.0f)));
				boxes.push_back(Scene.Factory.CreateBox(&SphereMaterial, model));
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

		static Assets::Mesh gPlane;
		Assets::Mesh::CreatePlane(&gPlane, Assets::MeshVertexDesc(), 100.0f, 100.0f);

		BoxPhysXMat.Create();

		//Assign Components
		EPlane.Assign<Components::MeshComponent>(&gPlane, &PlaneMaterial);
		EPlane.Assign<Components::ColliderComponent>(&BoxPhysXMat, PhysX::PlaneGeometry(ECS::Transform()));

		mPhysXSystem->AddActor(EPlane);

		Camera->GammaCorrection = true;
		Camera->HDR = true;
		Camera->MovementSpeed = 15;

		Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
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
		Graphics::Context::GetSwapChain()->Resize(width, height);
		Camera->SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		Camera->ResizeRenderTarget(width, height);
	}

	void Update(float deltatime) override
	{
		//Movement
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_FORWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_LEFT, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_BACKWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera->ProcessMovement(Components::CAMERA_MOVEMENT_RIGHT, deltatime);

		//Change Mouse Mode
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = true;
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);
		}
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_M) == Core::Input::KeyboardKeyStatus::Pressed)
		{
			isMouseDisabled = false;
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
		}

		Camera->Update();
		SceneCameraManager.UpdateBuffer();

		Camera->UpdatePSO();
	}
	bool iskinematic = false;

	void Render(float dt) override
	{
		mPhysXSystem->BeginSimulation(dt);
		mPhysXSystem->Update(Scene.Entities, Scene.Events, dt);

		ECamera.GetComponent<Components::SpotLightComponent>()->SetPosition(Camera->GetPosition());
		ECamera.GetComponent<Components::SpotLightComponent>()->SetDirection(Camera->GetFrontView());

		Renderer->Update(Scene.Entities, Scene.Events, dt);

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
			ImGui::Begin("Raycast Info");

			if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_F) == Core::Input::KeyboardKeyStatus::Pressed)
			{

				if (mPhysXSystem->Raycast(Camera.Get()->GetPosition(), Camera.Get()->GetFrontView(), 100.f, hit))
				{
					auto entity = hit.HitEntity;
					ECS::ComponentHandle<Components::EntityInfoComponent> Einfo = entity.GetComponent<Components::EntityInfoComponent>();
					ImGui::Text((char*)Einfo.Get()->mName.c_str());
				}
			}


			ImGui::End();


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
				return Core::Engine::EndGame();
			}

			ImGui::End();
			EntityExplorer(&Scene);

		}
		//ViewMaterialInfo(NanosuitMaterial, &AssetLoader);

	}
};