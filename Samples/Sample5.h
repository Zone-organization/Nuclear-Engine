#pragma once
#include "SampleBase.h"
void TextureVi2ewer(Assets::Texture* img)
{
	ImGui::Begin("Asset Info");

	ImVec2 tex_sz(128.f, 128.f);
	ImGui::Text(SplitFilename(img->GetName()).c_str());

	ImGui::Separator();
	ImGui::Image(img->GetTextureView(), tex_sz);

	ImGui::Text("Path: %s", img->GetName().c_str());
	ImGui::Text("Width: %i", img->GetWidth(), " - Height: %i", img->GetHeight());


	ImGui::End();
}

void AssetLibraryVi2ewer()
{
	ImGui::Begin("Assets Library Viewer");
	ImVec2 tex_sz(128.f, 128.f);

	if (ImGui::BeginTabBar("Asset Library"))
	{
		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		if (ImGui::BeginTabItem("Textures"))
		{
			static int count = 0;
			static Assets::Texture* img = nullptr;
			for (auto& i : Assets::AssetLibrary::Get().mImportedTextures.mData)
			{
				ImGui::PushID(count);
				if (i.second.GetTextureView())
				{
					ImGui::BeginGroup();
					{
						ImGui::Image(i.second.GetTextureView(), tex_sz);
						ImGui::Text(SplitFilename(i.second.GetName()).c_str());
						ImGui::EndGroup();
						if (ImGui::IsItemClicked())
							img = &i.second;
					}
				}
				else
				{
					//ERROR
				}

				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + tex_sz.x; // Expected position if next button was on same line
				if (count + 1 < Assets::AssetLibrary::Get().mImportedTextures.mData.size() && next_button_x2 < window_visible_x2)
					ImGui::SameLine();
				ImGui::PopID();

				count++;
			}
			count = 0;
			ImGui::EndTabItem();

			if (img)
			{
				//if (img->GetState() == Assets::Asset::State::Created)
				{
					TextureVi2ewer(img);
				}
			}
		}
		if (ImGui::BeginTabItem("Meshes"))
		{
			//Should show mesh icon only
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Materials"))
		{
			//Should show material rendered on a sphere
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}


class Sample5 : public SampleBase
{
	std::shared_ptr<Systems::PhysXSystem> mPhysXSystem;

	//Assets::Material RustedIron;
	Assets::Material* Plastic;

	Assets::Shader* PBR;
	Rendering::ForwardRenderingPath ForwardRP;

	Rendering::GeometryPass GeoPass;
	//	Rendering::PostProcessingPass PostFXPass;

	std::vector<ECS::Entity> boxes;

public:
	Sample5()
	{

	}
	void SetupAssets()
	{
		GetAssetManager().LoadFolder("@Assets@/Textures/PBR/Plastic/");
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

		//PostFXPass.Bake({ _Width_, _Height_,Rendering::RenderingModule::Get().GetFinalRT().GetDesc(});
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

				auto sphere = GetScene().CreateSphere(Plastic, ESphere);
				position.z += 5.0f;

				//ECS::Transform EBox(position, Math::Vector3(1.0f));

				//boxes.push_back(GetScene().GetFactory().CreateBox(&SphereMaterial, EBox));
			}
		}

		GetScene().CreatePlane(Plastic);
		for (auto& it : boxes)
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

		Platform::Input::Get().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
	}

	bool iskinematic = false;

	void Render(float dt) override
	{
		GetScene().Update(dt);

		EController.GetComponent<Components::LightComponent>().SetDirection(GetScene().GetMainCamera()->GetFrontView());


		{
			using namespace Graphics;
			ImGui::Begin("Sample5: Async Assets Loading");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			if (ImGui::Button("End Game"))
			{
				ImGui::End();
				return Core::Engine::Get().EndClient();
			}

			ImGui::End();
			EntityExplorer();
			AssetLibraryVi2ewer();

		}
	}

	void Shutdown() override
	{
		GetAssetManager().FlushContainers();
	}
};