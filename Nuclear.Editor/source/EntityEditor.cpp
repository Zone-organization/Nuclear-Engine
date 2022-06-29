#include "EntityEditor.h"
namespace Nuclear::Editor
{
	template <class ...Component>
	std::vector<entt::id_type> GetAllComponentsHashes()
	{
		std::vector<entt::id_type> result;
		((result.push_back(entt::type_id<Component>().hash())), ...);
		return result;
	}
	EntityEditor::EntityEditor()
	{
		AllComponentsIDs = GetAllComponentsHashes<ALL_COMPONENTS>();
	}


	template<typename... Components>
	std::vector<entt::id_type> GetAvailableComponents(entt::entity& entity, entt::registry& reg, const std::vector<entt::id_type>& AllComponentsIDs)
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
		(AddComponentByHashImp<Component>(entity, reg, id), ...);
	}

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


	void EntityEditor::Render(entt::entity& entity, entt::registry& reg, Components::EntityInfoComponent& Einfo)
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
					Components::DirLightComponent* light = reg.try_get<Components::DirLightComponent>(entity);
					static bool show = false;
					if (light != nullptr)
					{
						show = true;
					}

					if (ImGui::CollapsingHeader("Directional Light", &show))
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
					if (light != nullptr && show == false)
					{
						reg.erase<Components::DirLightComponent>(entity);						
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
					auto avail_comps = GetAvailableComponents<ALL_COMPONENTS>(entity, reg, AllComponentsIDs);

					ImGui::Text("Available Components:");
					ImGui::Separator();

					for (auto i : avail_comps)
					{
						if (ImGui::Selectable(GetComponentName(i).c_str()))
						{
							AddComponentByHash<ALL_COMPONENTS>(entity, reg, i);
						}
					}

					ImGui::EndPopup();
				}

			}
			ImGui::PopID();


		}
		ImGui::End();
	}
}
