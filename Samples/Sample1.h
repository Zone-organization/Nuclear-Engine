#pragma once
#include "Common.h"

std::string SplitFilename(const std::string& str)
{
	std::size_t found = str.find_last_of("/");
	return str.substr(found + 1);
}

void ImageViewer(Assets::Image* img)
{
	ImGui::Begin("Asset Info");

	ImVec2 tex_sz(128.f, 128.f);
	ImGui::Text(SplitFilename(img->GetName()).c_str());

	ImGui::Separator();
	ImGui::Image(img->mTextureView, tex_sz);

	ImGui::Text("Path: %s", img->GetName().c_str());
	ImGui::Text("Width: %i", img->mData.mWidth, " - Height: %i", img->mData.mHeight);


	ImGui::End();
}

void AssetLibraryViewer(Assets::AssetLibrary& obj)
{
	ImGui::Begin("Assets Library Viewer");
	ImVec2 tex_sz(128.f, 128.f);

	if (ImGui::BeginTabBar("Asset Library"))
	{
		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		if (ImGui::BeginTabItem("Images"))
		{
			static int count = 0;
			static Assets::Image* img = nullptr;
			for (auto& i : obj.mImportedImages.mData)
			{
				ImGui::PushID(count);
				if (i.second.mTextureView)
				{
					ImGui::BeginGroup();
					{
						ImGui::Image(i.second.mTextureView, tex_sz);
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
				if (count + 1 < obj.mImportedImages.mData.size() && next_button_x2 < window_visible_x2)
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
					ImageViewer(img);
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

void AssetsFolderViewer(Assets::AssetLibrary& obj)
{

}

class Sample1 : public Core::Client
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
//	std::shared_ptr<Systems::DebugSystem> mDebugSystem;


	Assets::Material CubeMaterial;
	Assets::Material NanosuitMaterial;
	Assets::Material CyborgMaterial;
	Assets::Material BobMaterial;
	Assets::Material VampireMaterial;

	Assets::MaterialData CubeTextures;

	Animation::Animator BobAnimator;
	Animation::Animator VampireAnimator;

	Assets::Shader* DiffuseOnly;
	Assets::Shader* BlinnPhong;

	Rendering::Skybox Skybox;

	Rendering::ForwardRenderingPath ForwardRP;

	Rendering::GeometryPass GeoPass;

	ECS::Entity ECube;
	ECS::Entity ECyborg;
	ECS::Entity ENanosuit;
	ECS::Entity EBob;
	ECS::Entity EVampire;

	ECS::Entity EController;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;
	bool isMouseDisabled = false;

public:
	Sample1()
	{
	}
	void SetupAssets()
	{
		Assets::ModelImportingDesc ModelDesc;

		//Load Nanosuit Model
		auto nanosuitmodel = GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/CrytekNanosuit/nanosuit.obj", ModelDesc);
		
		//Load Cyborg Model
		auto cyborgmodel = GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/CrytekCyborg/cyborg.obj", ModelDesc);
		
		//Load Bob Model
		auto bobmodel = GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/Bob/boblampclean.md5mesh", ModelDesc);

		//Load vampire Model
		auto vampiremodel = GetAssetManager().Import<Assets::Model>("@CommonAssets@/Models/vampire/vampire_a_lusth.fbx", ModelDesc);

		BobAnimator.Initialize(&bobmodel->pAnimations->mClips.at(0));
		VampireAnimator.Initialize(&vampiremodel->pAnimations->mClips.at(0));

		//Load some textures manually
		Assets::ImageImportingDesc desc;
	//	desc.mFormat = TEX_FORMAT_RGBA8_UNORM;

		//Initialize Materials
		Assets::TextureSet CubeSet;
		CubeSet.mData.push_back({ 0, GetAssetManager().ImportTexture("@CommonAssets@/Textures/crate_diffuse.png",{ Graphics::TextureUsageType::Diffuse}) });
		CubeSet.mData.push_back({ 1, GetAssetManager().ImportTexture("@CommonAssets@/Textures/crate_specular.png",{ Graphics::TextureUsageType::Specular}) });
		CubeSet.mData.push_back({ 2, GetAssetManager().ImportTexture("@CommonAssets@/Textures/crate_normal.png",{ Graphics::TextureUsageType::Normal}) });
		
		CubeTextures.mTextures.push_back(CubeSet);

		CubeMaterial.Create(&CubeTextures, BlinnPhong);
		NanosuitMaterial.Create(nanosuitmodel->pMaterialData, BlinnPhong);
		CyborgMaterial.Create(cyborgmodel->pMaterialData, BlinnPhong);
		BobMaterial.Create(bobmodel->pMaterialData, DiffuseOnly);
		VampireMaterial.Create(vampiremodel->pMaterialData, BlinnPhong);

		CubeSet.mData.clear();

		ECube.AddComponent<Components::MeshComponent>(Assets::DefaultMeshes::GetCubeAsset(),&CubeMaterial);
		ENanosuit.AddComponent<Components::MeshComponent>(nanosuitmodel->pMesh, &NanosuitMaterial);
		ECyborg.AddComponent<Components::MeshComponent>(cyborgmodel->pMesh, &CyborgMaterial);

		EBob.AddComponent<Components::MeshComponent>(bobmodel->pMesh,&BobMaterial, &BobAnimator);

		//EVampire.AddComponent<Components::MeshComponent>(VampireAsset, &VampireMaterial);
		//EVampire.AddComponent<Components::AnimatorComponent>(&VampireAnimator);

		//Create The skybox
		std::array<Core::Path, 6> SkyBoxTexturePaths
		{
			Core::Path("@CommonAssets@/Skybox/right.jpg"),
			Core::Path("@CommonAssets@/Skybox/left.jpg"),
			Core::Path("@CommonAssets@/Skybox/top.jpg"),
			Core::Path("@CommonAssets@/Skybox/bottom.jpg"),
			Core::Path("@CommonAssets@/Skybox/front.jpg"),
			Core::Path("@CommonAssets@/Skybox/back.jpg")
		};

		Assets::ImageImportingDesc SkyboxDesc;
		//SkyboxDesc.mFormat = TEX_FORMAT_RGBA8_UNORM;
		auto test =Assets::Importer::GetInstance().ImportTextureCube(SkyBoxTexturePaths,&GetAssetManager().GetDefaultLibrary(), SkyboxDesc);
		Skybox.Initialize(test);
	}
	void SetupEntities()
	{
		//Create Entities
		ECS::Transform TNansosuit, TCyborg, TBob, TVampire, TCube;
		TNansosuit.SetPosition(Math::Vector3(0.0f, -1.75f, 0.0f));
		TNansosuit.SetScale(Math::Vector3(0.3f, 0.3f, 0.3f));

		TCyborg.SetPosition(Math::Vector3(4.0f, -1.75f, 0.0f));

		TBob.SetPosition(Math::Vector3(-4.0f, -1.75f, 0.0f));
		TBob.SetScale(Math::Vector3(0.07f, 0.07f, 0.07f));

		TVampire.SetPosition(Math::Vector3(-4.0f, -1.75f, 4.0f));
		TVampire.SetScale(Math::Vector3(0.02f, 0.02f, 0.02f));

		TCube.SetPosition(Math::Vector3(2.0f, -1.75f, 2.0f));
		TCube.SetScale(Math::Vector3(2.f, 2.f, 2.f));

		ECube = GetScene().CreateEntity("Cube", TCube);
		ENanosuit = GetScene().CreateEntity("Nanosuit", TNansosuit);
		ECyborg = GetScene().CreateEntity("Cyborg", TCyborg);
		EBob = GetScene().CreateEntity("Bob", TBob);
	//	EVampire = GetScene().CreateEntity("Vampire" , TVampire);


		//Create Entities
		auto EDirLight = GetScene().CreateEntity("DirLight");
		auto& dircomp = EDirLight.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Directional);
		dircomp.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dircomp.SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		auto ELights = GetScene().CreateEntity("PointLight1");
		auto& lightcomp = ELights.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Point);

		ELights.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(Math::Vector3(0.7f, 0.2f, 2.0f));
	}

	void InitRenderer()
	{
	//	mDebugSystem = GetScene().GetSystemManager().Add<Systems::DebugSystem>();
		Renderer = GetScene().GetSystemManager().Add<Systems::RenderSystem>();

		Assets::ShaderImportingDesc desc;
		desc.mType = Assets::ShaderType::_3DRendering;
		BlinnPhong = GetAssetManager().Import<Assets::Shader>("@NuclearAssets@/Shaders/BlinnPhong.NEShader", desc);
		DiffuseOnly = GetAssetManager().Import<Assets::Shader>("@NuclearAssets@/Shaders/DiffuseOnly.NEShader", desc);

		Renderer->RegisterShader(BlinnPhong);
		Renderer->RegisterShader(DiffuseOnly);

		Systems::RenderSystemBakingDesc bakedesc;
		bakedesc.RTWidth = _Width_;
		bakedesc.RTHeight = _Height_;
		Renderer->Bake(bakedesc);
	}

	void Load()
	{
		GetAssetManager().Initialize();

		EController = GetScene().CreateEntity();
		EController.AddComponent<Components::LightComponent>(Components::LightComponent::Type::Spot);

		//Initialize camera
		{
			Components::CameraComponentDesc cameradesc;
			cameradesc.mProjection = Math::perspective(Math::radians(45.0f), Core::Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f);
			cameradesc.mRTDesc = Rendering::RenderingEngine::GetInstance().GetFinalRT().GetDesc();
			cameradesc.mDepthRTDesc = Rendering::RenderingEngine::GetInstance().GetFinalDepthRT().GetDesc();
			GetScene().SetMainCamera(&EController.AddComponent<Components::CameraComponent>(cameradesc));
		}

		SetupEntities();

		InitRenderer();

		SetupAssets();

		Renderer->AddRenderPass(&GeoPass);
	//	Renderer->GetBackground().SetSkybox(&Skybox);

		Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
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

			GetScene().GetMainCamera()->ProcessEye(xoffset, yoffset);
		}
	}


	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetInstance().GetSwapChain()->Resize(width, height);
		GetScene().GetMainCamera()->ResizeRTs(width, height);
		GetScene().GetMainCamera()->SetProjectionMatrix(Math::perspective(Math::radians(45.0f), Core::Engine::GetInstance().GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		GetScene().GetSystemManager().ResizeRTs(width, height);
		Rendering::RenderingEngine::GetInstance().ResizeRTs(width, height);
	}
	void Update(float deltatime) override
	{
		//Movement
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_W))
			GetScene().GetMainCamera()->ProcessMovement(Components::CAMERA_MOVEMENT_FORWARD, deltatime);
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_A))
			GetScene().GetMainCamera()->ProcessMovement(Components::CAMERA_MOVEMENT_LEFT, deltatime);
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_S))
			GetScene().GetMainCamera()->ProcessMovement(Components::CAMERA_MOVEMENT_BACKWARD, deltatime);
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_D))
			GetScene().GetMainCamera()->ProcessMovement(Components::CAMERA_MOVEMENT_RIGHT, deltatime);

		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_LSHIFT))
			GetScene().GetMainCamera()->MovementSpeed = 10;
		else
			GetScene().GetMainCamera()->MovementSpeed = 4.5;

		//Change Mouse Mode
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_ESCAPE))
		{
			isMouseDisabled = true;
			Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Normal);
		}
		if (Platform::Input::GetInstance().IsKeyPressed(Platform::Input::KEYCODE_M))
		{
			isMouseDisabled = false;
			Platform::Input::GetInstance().SetMouseInputMode(Platform::Input::MouseInputMode::Locked);
		}

		GetScene().GetMainCamera()->UpdateBuffer();
		EController.GetComponent<Components::EntityInfoComponent>().mTransform.SetPosition(GetScene().GetMainCamera()->GetPosition());

	//	Renderer->GetActivePipeline()->UpdatePSO();
	}
	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };
		BobAnimator.UpdateAnimation(dt);
	//	VampireAnimator.UpdateAnimation(dt);

		EController.GetComponent<Components::LightComponent>().SetDirection(GetScene().GetMainCamera()->GetFrontView());

		GetScene().Update(dt);
		{
			using namespace Graphics;
			ImGui::Begin("Sample1: Basic Rendering");

			ImGui::Text("Press M to enable mouse capturing, or Esc to disable mouse capturing");

			ImGui::ColorEdit3("Camera ClearColor", (float*)&GetScene().GetMainCamera()->mRTClearColor);

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
			AssetLibraryViewer(GetAssetManager().GetDefaultLibrary());

			//mDebugSystem->ShowRendertargets();
		}
	}

	void Shutdown() override
	{
		GetAssetManager().FlushContainers();
	}
};