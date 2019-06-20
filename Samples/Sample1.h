#pragma once
#include "Common.h"
class Sample1 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	Core::Input Input;

	RefCntAutoPtr<IShaderResourceBinding> mSRB;

	//Asset Manager (Loader)
	Managers::AssetManager AssetLoader;

	//Assets
	Assets::Texture DiffuseTex;
	Assets::Texture SpecularTex;
	Assets::Texture WhiteTex;
	Assets::Texture GreyTex;
	Assets::Texture GridTex;

	Assets::Mesh CubeAsset;
	Assets::Mesh SphereAsset;
	Assets::Mesh* NanosuitAsset;
	Assets::Mesh GridAsset;

	Assets::Material CubeMaterial;
	Assets::Material SphereMaterial;
	Assets::Material* NanosuitMaterial;
	Assets::Material GridMaterial;

	//Default states
	//Graphics::API::CommonStates states;

	Components::CameraComponent Camera;

	Components::DirectionalLight dirlight;
	Components::PointLight pointlight1;
	Components::PointLight pointlight2;
	Components::PointLight pointlight3;
	Components::PointLight pointlight4;
	Components::PointLight pointlight5;
	Components::PointLight pointlight6;
	Components::PointLight pointlight7;
	Components::PointLight pointlight8;
	Components::PointLight pointlight9;

	Components::SpotLight spotLight;

	Graphics::Skybox Skybox;

	Graphics::DiffuseOnly DiffuseRP;

	//ECS
	ECS::Scene SampleScene;
	ECS::Entity ESkybox;
	ECS::Entity ECube;
	ECS::Entity ELamp;
	ECS::Entity ENanosuit;
	ECS::Entity EGrid;

	// positions all containers
	Math::Vector3 cubePositions[10] =
	{
		Math::Vector3(0.0f,  0.0f,  0.0f),
		Math::Vector3(2.0f,  5.0f, -15.0f),
		Math::Vector3(-1.5f, -2.2f, -2.5f),
		Math::Vector3(-3.8f, -2.0f, -12.3f),
		Math::Vector3(2.4f, -0.4f, -3.5f),
		Math::Vector3(-1.7f,  3.0f, -7.5f),
		Math::Vector3(1.3f, -2.0f, -2.5f),
		Math::Vector3(1.5f,  2.0f, -2.5f),
		Math::Vector3(1.5f,  0.2f, -1.5f),
		Math::Vector3(-1.3f,  1.0f, -1.5f)
	};

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
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;


	void SetupLights()
	{
		dirlight.SetDirection(Math::Vector3(-0.2f, -1.0f, -0.3f));
		dirlight.SetColor(Graphics::Color(0.4f, 0.4f, 0.4f, 0.0f));

		pointlight1.SetPosition(pointLightPositions[0]);
		pointlight1.SetColor(Graphics::Color(1.0f, 1.0f, 1.0f, 0.0f));

		pointlight2.SetPosition(pointLightPositions[1]);
		pointlight2.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight3.SetPosition(pointLightPositions[2]);
		pointlight3.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight4.SetPosition(pointLightPositions[3]);
		pointlight4.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight5.SetPosition(pointLightPositions[4]);
		pointlight5.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight6.SetPosition(pointLightPositions[5]);
		pointlight6.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight7.SetPosition(pointLightPositions[6]);
		pointlight7.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight8.SetPosition(pointLightPositions[7]);
		pointlight8.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));

		pointlight9.SetPosition(pointLightPositions[8]);
		pointlight9.SetColor(Graphics::Color(0.8f, 0.8f, 0.8f, 0.0f));
	}

	void SetupAssets()
	{
		//Create some shapes
		Assets::Mesh::CreateCube(&CubeAsset);
		//Assets::Mesh::CreateSphere(&SphereAsset);
		//Assets::Mesh::CreateGrid(&GridAsset, Assets::MeshVertexDesc(), 6, 6, 10, 10);

		//NanoSuit Creation
		//Importers::MeshLoadingDesc ModelDesc;
		//ModelDesc.LoadDiffuseTextures = true;
		//ModelDesc.LoadSpecularTextures = true;
		//std::tie(NanosuitAsset, NanosuitMaterial) = AssetLoader.Import("Assets/Common/Models/CrytekNanosuit/nanosuit.obj", ModelDesc);
		//AssetLoader.mImportedMeshes[0] = CubeAsset;

		//Load some textures manually
		Importers::TextureLoadingDesc desc;
		desc.mFormat = TEX_FORMAT_RGBA8_UNORM;
		DiffuseTex = AssetLoader.Import("Assets/Common/Textures/crate_diffuse.png", Assets::TextureUsageType::Diffuse);
		SpecularTex = AssetLoader.Import("Assets/Common/Textures/crate_specular.png", Assets::TextureUsageType::Specular);
		//WhiteTex = AssetLoader.Import("Assets/Common/Textures/white.png");
		//GreyTex = AssetLoader.Import("Assets/Common/Textures/grey.png");
		//GridTex = AssetLoader.Import("Assets/Common/Textures/Grid.png");

		//Initialize Materials
		Assets::TextureSet CubeSet;
		CubeSet.push_back({ 0, DiffuseTex });
		//CubeSet.push_back({ 1, SpecularTex });
		CubeMaterial.mPixelShaderTextures.push_back(CubeSet);

		//Assets::TextureSet SphereTextures;
		//WhiteTex.SetUsageType(Assets::TextureUsageType::Diffuse);
		//SphereTextures.push_back({ 0,WhiteTex });
		//WhiteTex.SetUsageType(Assets::TextureUsageType::Specular);
		//SphereTextures.push_back({ 0,WhiteTex });
		//SphereMaterial.mPixelShaderTextures.push_back(SphereTextures);


		//Assets::TextureSet GridTextures;
		//GridTex.SetUsageType(Assets::TextureUsageType::Diffuse);
		//GridTextures.push_back({ 0,GridTex });
		//GreyTex.SetUsageType(Assets::TextureUsageType::Specular);
		//GridTextures.push_back({ 0,GreyTex });
		//GridMaterial.mPixelShaderTextures.push_back(GridTextures);

		//Renderer->CreateMaterial(&GridMaterial);
		Renderer->CreateMaterial(&CubeMaterial);
		//Renderer->CreateMaterial(&SphereMaterial);
		//Renderer->CreateMaterial(NanosuitMaterial);

		CubeSet.clear();
		//SphereTextures.clear();
		//GridTextures.clear();

		//CubeMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//CubeMaterial.SetMaterialVariable("Shininess", 64.0f);

		//SphereMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//SphereMaterial.SetMaterialVariable("Shininess", 64.0f);

		//NanosuitMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//NanosuitMaterial.SetMaterialVariable("Shininess", 64.0f);

		//GridMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		//GridMaterial.SetMaterialVariable("Shininess", 64.0f);

		//Create The skybox
		//std::array<std::string, 6> SkyBoxTexturePaths
		//{
		//	std::string("Assets/Common/Skybox/right.jpg"),
		//	std::string("Assets/Common/Skybox/left.jpg"),
		//	std::string("Assets/Common/Skybox/top.jpg"),
		//	std::string("Assets/Common/Skybox/bottom.jpg"),
		//	std::string("Assets/Common/Skybox/front.jpg"),
		//	std::string("Assets/Common/Skybox/back.jpg")
		//};

		//Skybox.Initialize(&Camera, SkyBoxTexturePaths);
	}

	//void ChooseRenderingPipeline()
	//{
	//	std::cout << "Select Rendering Pipeline: \n"
	//		<< "1) DiffuseOnly \n"
	//		<< "2) BlinnPhong \n";

	//	int i;
	//	std::cin >> i;

	//	switch (i)
	//	{
	//	case 1:
	//		//return Core::RenderAPI::DirectX11;
	//	case 2:
	//	//	return Core::RenderAPI::DirectX12;
	//	}
	//}

	void SetupEntities()
	{
		//Create Entities
		ECube = SampleScene.CreateEntity();
		ELamp = SampleScene.CreateEntity();
		EGrid = SampleScene.CreateEntity();
		ENanosuit = SampleScene.CreateEntity();

		//Assign Components
		ECube.Assign<Components::MeshComponent>(&CubeAsset, &CubeMaterial, true);
		ELamp.Assign<Components::MeshComponent>(&SphereAsset, &SphereMaterial, true);
		EGrid.Assign<Components::MeshComponent>(&GridAsset, &GridMaterial);
		ENanosuit.Assign<Components::MeshComponent>(NanosuitAsset, NanosuitMaterial);
	}


	void Load()
	{
		Systems::RenderSystemDesc desc;
		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc, &Camera);
		SampleScene.Systems.Configure();

		Renderer->SetRenderingPipeline(&DiffuseRP);

		Renderer->GetCamera()->Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetMainWindow()->GetAspectRatioF32(), 0.1f, 100.0f));
		Renderer->AddLight(&spotLight);
		Renderer->AddLight(&pointlight1);
		Renderer->AddLight(&pointlight2);
		Renderer->AddLight(&pointlight3);
		Renderer->AddLight(&pointlight4);
		Renderer->AddLight(&pointlight5);
		Renderer->AddLight(&pointlight6);
		Renderer->AddLight(&pointlight7);
		Renderer->AddLight(&pointlight8);
		Renderer->AddLight(&pointlight9);
		Renderer->AddLight(&dirlight);
		Renderer->Bake();

		SetupLights();

		SetupAssets();

		SetupEntities();

		//NanosuitMaterial->GetPipeline()->CreateShaderResourceBinding(&mSRB, true);

		//mSRB->GetVariable(SHADER_TYPE_PIXEL, "NEMat_Specular1")->Set(SpecularTex.mTexture);

		//mSRB->GetVariable(SHADER_TYPE_PIXEL, "NEMat_Diffuse1")->Set(DiffuseTex.mTexture);
		//mSRB->GetVariable(SHADER_TYPE_PIXEL, "NEMat_Diffuse1")->Set(SpecularTex.mTexture);


		Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);
	}

	void OnMouseMovement(int xpos_a, int ypos_a) override
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

		Camera.ProcessEye(xoffset, yoffset);
	}

	void Update(float deltatime) override
	{
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_W) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_A) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_S) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_D) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_LEFT_SHIFT) == Core::Input::KeyboardKeyStatus::Pressed)
			Camera.MovementSpeed = 6;
		else
			Camera.MovementSpeed = 2.5;

		//Change Mouse Mode
		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_T) == Core::Input::KeyboardKeyStatus::Pressed
			|| Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_ESCAPE) == Core::Input::KeyboardKeyStatus::Pressed)
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Normal);

		if (Core::Application::GetMainWindow()->GetInput()->GetKeyStatus(Core::Input::KeyboardKey::KEY_Y) == Core::Input::KeyboardKeyStatus::Pressed)
			Core::Application::GetMainWindow()->GetInput()->SetMouseInputMode(Core::Input::MouseInputMode::Virtual);

		Camera.Update();
	}
	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };
		Graphics::Context::GetContext()->ClearRenderTarget(nullptr, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(nullptr, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		//Renderer->Update_Light();

		Graphics::Context::GetContext()->SetPipelineState(Renderer->GetPipeline());

		Math::Matrix4 model(1.0f);
		//model = Math::translate(model, Math::Vector3(0.0f, -1.75f, 0.0f));
		//model = Math::scale(model, Math::Vector3(0.2f, 0.2f, 0.2f));
		//Camera.SetModelMatrix(model);
		//Renderer->InstantRender(ENanosuit.GetComponent<Components::MeshComponent>().Get());


		model = Math::translate(model, Math::Vector3(2.0f, -1.75f, 2.0f));
		Camera.SetModelMatrix(model);
		Renderer->InstantRender(ECube.GetComponent<Components::MeshComponent>().Get());


		Graphics::Context::GetSwapChain()->Present();
	}
};