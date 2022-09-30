#pragma once
#include <NuclearEngine.h>

using namespace Nuclear;
const int _Width_ = 1280;
const int _Height_ = 720;
void EntityView(entt::entity& entity, entt::registry& reg, Components::EntityInfoComponent& Einfo);

void EntityExplorer(Assets::Scene* scene)
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
				EntityView(entity, scene->GetRegistry(), Einfo);
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

					float f = light->GetIntensity();
					if (ImGui::SliderFloat("Intensity", &f, 0.0f, 100.0f, "%.4f", ImGuiSliderFlags_None))
					{
						light->SetIntensity(f);
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

					float np = light->GetNearPlane();
					if (ImGui::SliderFloat("Near Plane", &np, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_None))
					{
						light->SetNearPlane(np);
					}

					float fp = light->GetFarPlane();
					if (ImGui::SliderFloat("Far Plane", &fp, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_None))
					{
						light->SetFarPlane(fp);
					}

					float fov = light->GetFOV();
					if (ImGui::SliderFloat("Fov", &fov, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_None))
					{
						light->SetFOV(fov);
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

					float np = light->GetNearPlane();
					if (ImGui::SliderFloat("Near Plane", &np, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_None))
					{
						light->SetNearPlane(np);
					}

					float fp = light->GetFarPlane();
					if (ImGui::SliderFloat("Far Plane", &fp, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_None))
					{
						light->SetFarPlane(fp);
					}

					float fov = light->GetFOV();
					if (ImGui::SliderFloat("Fov", &fov, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_None))
					{
						light->SetFOV(fov);
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

						for (auto &it : meshcomponent->mMaterial->mPixelShaderTextures)
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

		ImGui::PopID();
	}
	ImGui::End();
}

