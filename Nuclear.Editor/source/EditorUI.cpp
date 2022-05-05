#include "EditorUI.h"
#include "Nuclear.Editor.h"
#include <type_traits>
#include <Engine/ECS/entt/core/type_info.hpp>
using namespace entt::literals;

#define COMPONENTS 	Components::EntityInfoComponent,\
					Components::CameraComponent,    \
					Components::MeshComponent,      \
					Components::ColliderComponent,  \
					Components::RigidBodyComponent, \
					Components::DirLightComponent,  \
					Components::PointLightComponent,\
					Components::SpotLightComponent, \
					Components::AnimatorComponent,  \
					Components::ScriptComponent    

namespace Nuclear::Editor 
{
	static std::vector<entt::id_type> AllComponentsIDs;
	template <class ...Component>
	std::vector<entt::id_type> GetAllComponentsHashes()
	{
		std::vector<entt::id_type> result;
		((result.push_back(entt::type_id<Component>().hash())), ...);
		return result;
	}

	EditorUI::EditorUI(NuclearEditor* editor)
	{
		mEditorInstance = editor;
		pActiveProject = nullptr;
		AllComponentsIDs = GetAllComponentsHashes<COMPONENTS>();
	}
	void EditorUI::SetProject(Project* project)
	{
		pActiveProject = project;
	}
	void EditorUI::Render()
	{
		RenderMainMenuBar();

		if (mScene)
		{
			RenderEntityExplorer();
		}
	}

	void EditorUI::RenderMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			//Scene / Project
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("New"))
				{
					if (pActiveProject != nullptr)
					{
						if (ImGui::MenuItem("Scene"))
						{
							//auto sceneptr = pActiveProject->AddNewScene();
							//if (mScene == nullptr) { mScene = sceneptr; }
						}
						ImGui::Separator();
					}

					if(ImGui::MenuItem("Project"))
					{
						pActiveProject = &mEditorInstance->mActiveProject;
						mScene = pActiveProject->GetActiveScene();
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}


	template<typename... Components>
	std::vector<entt::id_type> GetAvailableComponents(entt::entity& entity, entt::registry& reg)
	{
		std::vector<entt::id_type> all_comp_vec(AllComponentsIDs);

		std::vector<entt::id_type> entity_comp_vec;
		for (auto [id, storage] : reg.storage())
		{
			if (storage.contains(entity))
			{
				//id is hashed component 
				entity_comp_vec.push_back(id);
			}
		}

		//delete added components
		for (auto j : entity_comp_vec)
		{
			for (auto it = all_comp_vec.begin(); it != all_comp_vec.end();)
			{
				if (*it == j)
					it = all_comp_vec.erase(it);
				else
					++it;

			}
		}

		return all_comp_vec;
	}

	template<typename Component>
	const void AddComponentByHashImp(entt::entity& entity, entt::registry& reg, entt::id_type id)
	{
		if (entt::type_id<Component>().hash() == id)
		{
			reg.emplace<Component>(entity);
		}
	}

	template<typename... Component>
	const void AddComponentByHash(entt::entity& entity, entt::registry& reg, entt::id_type id)
	{
		( AddComponentByHashImp<Component>(entity,reg,id), ...);
	}


	//template<typename Component>
	//const void AddComponentIfDoesntExist(entt::entity& entity, entt::registry& reg, Component& component)
	//{
	//	if (!reg.all_of<Component>(entity))
	//	{
	//		return reg.emplace<Component>(entity);
	//	}
	//}

	//For display only
	const std::string GetComponentName(entt::id_type id)
	{
		if (id == entt::type_id<Components::EntityInfoComponent>().hash())	
			return "Entity Info Component";		
		else if (id == entt::type_id<Components::CameraComponent>().hash())
			return "Camera Component";
		else if (id == entt::type_id<Components::MeshComponent>().hash())
			return "Mesh Component";
		else if (id == entt::type_id<Components::ColliderComponent>().hash())
			return "Collider Component";
		else if (id == entt::type_id<Components::RigidBodyComponent>().hash())
			return "Rigid Body Component";
		else if (id == entt::type_id<Components::DirLightComponent>().hash())
			return "Directional Light Component";
		else if (id == entt::type_id<Components::PointLightComponent>().hash())
			return "Point Light Component";
		else if (id == entt::type_id<Components::SpotLightComponent>().hash())
			return "Spot Light Component";
		else if (id == entt::type_id<Components::AnimatorComponent>().hash())
			return "Animator Component";
		else if (id == entt::type_id<Components::ScriptComponent>().hash())
			return "Script Component";


		return "Unknown";
	}


	void EditorUI::RenderEntityEditor(entt::entity& entity, entt::registry& reg, Components::EntityInfoComponent& Einfo)
	{
		auto index = (Uint32)entity;

		using namespace Graphics;
		ImGui::Begin("Entity Editor");
		{
			
				ImGui::PushID(index);           // Push i to the id tack
				{
					ImGui::InputText("Name", (char*)Einfo.mName.c_str(), Einfo.mName.capacity() + 1);
					if (ImGui::CollapsingHeader("Transform"))
					{
						Components::EntityInfoComponent* transform = reg.try_get< Components::EntityInfoComponent>(entity);

						Math::Vector3 pos = transform->mTransform.GetLocalPosition();
						ImGui::DragFloat3("Position", (float*)&pos, 0.2f);
						if (pos != transform->mTransform.GetLocalPosition())
						{
							transform->mTransform.SetPosition(pos);
						}

						Math::Vector3 rot = transform->mTransform.GetLocalRotationEular();
						ImGui::DragFloat3("Rotation", (float*)&rot, 0.2f);
						if (rot != transform->mTransform.GetLocalRotationEular())
						{
							transform->mTransform.SetRotation(rot);
						}

						Math::Vector3 scale = transform->mTransform.GetLocalScale();
						ImGui::DragFloat3("Scale", (float*)&scale, 0.2f);
						if (scale != transform->mTransform.GetLocalScale())
						{
							transform->mTransform.SetScale(scale);
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
							//if (ImGui::CollapsingHeader("Camera"))
							//{
							//	auto cameraptr = camera->GetCameraPtr();
							//	ImGui::ColorEdit3("RenderTarget ClearColor", (float*)&cameraptr->RTClearColor);


							//	for (auto it : cameraptr->GetAvailableCameraEffects())
							//	{
							//		ImGui::Checkbox(it.GetName().c_str(), &it.mValue);
							//	}
							//	//ImGui::DragFloat("Movement Speed", &Camera.MovementSpeed);
							//	//ImGui::DragFloat("Mouse Sensitivity", &Camera.MouseSensitivity);
							//	//ImGui::DragFloat("Zoom", &Camera.Zoom);


							//}
						}
					}

					//DirLight
					{
						Components::DirLightComponent* light = reg.try_get< Components::DirLightComponent>(entity);
						if (light)
						{
							if (ImGui::CollapsingHeader("Directional Light"))
							{
								Graphics::Color color = light->GetColor();
								if (ImGui::ColorEdit4("Dir Color", (float*)&color))
								{
									light->SetColor(color);
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
								Graphics::Color color = light->GetColor();
								if (ImGui::ColorEdit4("Point Color", (float*)&color))
								{
									light->SetColor(color);
								}

								ImVec4 att = ImVec4(light->GetAttenuation().x, light->GetAttenuation().y, light->GetAttenuation().z, 1.00f);
								if (ImGui::DragFloat3("Attenuation", (float*)&att))
								{
									light->SetAttenuation(Math::Vector3(att.x, att.y, att.z));
								}

								float f = light->GetIntensity();
								if (ImGui::SliderFloat("Intensity", &f, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_None))
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

								Graphics::Color color = light->GetColor();
								if (ImGui::ColorEdit4("Spot Color", (float*)&color))
								{
									light->SetColor(color);
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

								if (meshcomponent->mMesh != nullptr)
								{
									if (meshcomponent->mMesh->GetName() != std::string(""))
									{
										ImGui::Text(meshcomponent->mMesh->GetName().c_str());
									}
									else {
										ImGui::Text("Unnamed Mesh");
									}
								}
								if (meshcomponent->mMaterial != nullptr)
								{
									if (meshcomponent->mMaterial->GetName() != std::string(""))
									{
										ImGui::Text(meshcomponent->mMaterial->GetName().c_str());
									}
									else {
										ImGui::Text("Unnamed Material");
									}

									for (auto& it : meshcomponent->mMaterial->mPixelShaderTextures)
									{
										ImGui::Text(it.mName.c_str());

										for (auto& it1 : it.mData)
										{
											//ImGui::Text(it1.mSlot);
											if (it1.mTex.GetImage() != nullptr)
											{
												ImGui::Image(it1.mTex.GetImage()->mTextureView, { 256.f,256.f });
											}
										}
									}
								}

							}
						}
					}
				}

				//Add Component
				{
					// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
					if (ImGui::Button("Add Component.."))
						ImGui::OpenPopup("new_component_selector");
				//	ImGui::SameLine();

					if (ImGui::BeginPopup("new_component_selector"))
					{
						//First build vector of available components names
						auto avail_comps = GetAvailableComponents<COMPONENTS>(entity, reg);

						ImGui::Text("Available Components:");
						ImGui::Separator();

						for (auto i : avail_comps)
						{
							if (ImGui::Selectable(GetComponentName(i).c_str()))
							{
								AddComponentByHash<COMPONENTS>(entity, reg, i);
							}
						}

						ImGui::EndPopup();
					}

				}
				ImGui::PopID();
			

		}
		ImGui::End();
	}

	void EditorUI::RenderEntityExplorer()
	{
		ImGui::Begin("Entity Explorer");
		if (ImGui::TreeNode(mScene->GetName().c_str()))
		{
			auto view = mScene->GetRegistry().view<Components::EntityInfoComponent>();

			for (auto entity : view)
			{
				auto& Einfo = view.get<Components::EntityInfoComponent>(entity);
				auto index = (Uint32)entity;

				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

				static int selectedindex = 0;

				if (selectedindex == index)
				{
					node_flags |= ImGuiTreeNodeFlags_Selected;
					RenderEntityEditor(entity, mScene->GetRegistry(), Einfo);
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

		if(ImGui::Button("Add Entity"))
		{
			auto entity = mScene->CreateEntity();	
		}
		ImGui::End();
	}

}