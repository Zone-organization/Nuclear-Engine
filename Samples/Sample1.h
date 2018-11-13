#pragma once
#include "Common.h"

class Sample1 : public Core::Game
{
protected:
	std::shared_ptr<Systems::RenderSystem> Renderer;

	//Assets
	Assets::Texture DiffuseTex;
	Assets::Texture SpecularTex;
	Assets::Texture WhiteTex;

	Assets::Mesh CubeAsset;
	Assets::Mesh SphereAsset;
	Assets::Mesh NanosuitAsset;

	Assets::Material CubeMaterial;
	Assets::Material SphereMaterial;
	Assets::Material NanosuitMaterial;

	//Default states
	Graphics::API::CommonStates states;

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

	//ECS
	ECS::Scene SampleScene;
	ECS::Entity ESkybox;
	ECS::Entity ECube;
	ECS::Entity ELamp;
	ECS::Entity ENanosuit;

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
public:
	Sample1()
	{
	}

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
		//Cube Creation
		DiffuseTex = Managers::AssetManager::Import("Assets/Common/Textures/crate_diffuse.png", Assets::TextureUsageType::Diffuse);
		SpecularTex = Managers::AssetManager::Import("Assets/Common/Textures/crate_specular.png", Assets::TextureUsageType::Specular);
		std::vector<Assets::Texture> Textures;
		Textures.push_back(DiffuseTex);
		Textures.push_back(SpecularTex);

		Assets::Mesh::CreateCube(&CubeAsset, Textures);
		CubeAsset.Initialize(&Renderer->GetVertexShader());
		//CubeMaterial.mPixelShaderTextures = Textures;
		Textures.clear();

		//Sphere Creation
		WhiteTex = Managers::AssetManager::Import("Assets/Common/Textures/white.png");

		std::vector<Assets::Texture> SphereTextures;
		WhiteTex.SetUsageType(Assets::TextureUsageType::Diffuse);
		SphereTextures.push_back(WhiteTex);
		WhiteTex.SetUsageType(Assets::TextureUsageType::Specular);
		SphereTextures.push_back(WhiteTex);

		Assets::Mesh::CreateSphere(&SphereAsset, SphereTextures);
		SphereAsset.Initialize(&Renderer->GetVertexShader());

		//NanoSuit Creation
		Managers::MeshLoadingDesc ModelDesc;
		ModelDesc.LoadDiffuseTextures = true;
		ModelDesc.LoadSpecularTextures = true;
		NanosuitAsset = Managers::AssetManager::Import("Assets/Common/Models/CrytekNanosuit/nanosuit.obj", ModelDesc, NanosuitMaterial);
		NanosuitAsset.Initialize(&Renderer->GetVertexShader());

		NanosuitMaterial.SetPixelShader(Renderer->GetPixelShader());

		//NanosuitMaterial = Managers::AssetManager::mImportedMaterials.at(0);
		//Create The skybox
		std::array<std::string, 6> SkyBoxTexturePaths
		{
			std::string("Assets/Common/Skybox/right.jpg"),
			std::string("Assets/Common/Skybox/left.jpg"),
			std::string("Assets/Common/Skybox/top.jpg"),
			std::string("Assets/Common/Skybox/bottom.jpg"),
			std::string("Assets/Common/Skybox/front.jpg"),
			std::string("Assets/Common/Skybox/back.jpg")
		};

		Skybox.Initialize(&Camera, SkyBoxTexturePaths);
	}
	void SetupEntities()
	{
		//Create Entities
		//ECube = SampleScene.Entities.Create();
		//ELamp = SampleScene.Entities.Create();
		ENanosuit = SampleScene.Entities.Create();

		//Assign Components
		//ECube.Assign<Components::MeshComponent>(&CubeAsset , true);
		//ELamp.Assign<Components::MeshComponent>(&SphereAsset , true);
		ENanosuit.Assign<Components::TransformComponent>();

		ENanosuit.Assign<Components::MeshComponent>(&NanosuitAsset, &NanosuitMaterial);

	}
	void Load()
	{
		Systems::RenderSystemDesc desc;
		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc);
		SampleScene.Systems.Configure();

		//Camera.Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetAspectRatioF32(), 0.1f, 100.0f));

		Renderer->SetCamera(&Camera);
		Renderer->GetCamera()->Initialize(Math::perspective(Math::radians(45.0f), Core::Application::GetAspectRatioF32(), 0.1f, 100.0f));
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


		//mat.SetPixelShader(Renderer->GetPixelShader());

		Managers::AssetManager::mSaveTextureNames = true;

		SetupLights();

		SetupAssets();
	
		SetupEntities();

		states.EnabledDepth_DisabledStencil.Bind();
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
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
		//Core::Application::SetTitle("Nuclear Engine FPS: " + std::to_string(FPS) + " FrameTime: " + std::to_string(FrameTime));

		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::D))
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		Camera.Update();
	}
	void Render(float dt) override
	{
		Graphics::API::Context::Clear(Graphics::Color(0.1f, 0.1f, 0.1f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		Renderer->GetVertexShader().Bind();
		Renderer->GetPixelShader().Bind();

		states.DefaultSampler.PSBind(0);
		states.DefaultSampler.PSBind(1);

		std::vector<Math::Matrix4> CubeTransforms;
		std::vector<Math::Matrix4> LampTransforms;
		
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			Math::Matrix4 model(1.0f);
			model = Math::translate(model, cubePositions[i]);
			float angle = 20.0f * i * ClockTime;

			model = Math::rotate(model, Math::radians(angle), Math::Vector3(1.0f, 0.3f, 0.5f));
			CubeTransforms.push_back(model);
		}
		for (unsigned int i = 0; i < 9; i++)
		{
			Math::Matrix4 model(1.0f);
			model = Math::translate(model, pointLightPositions[i]);
			model = Math::scale(model, Math::Vector3(0.25f));
			LampTransforms.push_back(model);
		}

	//	ECube.GetComponent<Components::MeshComponent>().Get()->mMultiRenderTransforms = CubeTransforms;
	//	ELamp.GetComponent<Components::MeshComponent>().Get()->mMultiRenderTransforms = LampTransforms;
		
		Math::Matrix4 NanosuitMatrix(1.0f);
		NanosuitMatrix = Math::translate(NanosuitMatrix, Math::Vector3(5.0f, -1.75f, 0.0f));
		NanosuitMatrix = Math::scale(NanosuitMatrix, Math::Vector3(0.25f));
		NanosuitMatrix = Math::rotate(NanosuitMatrix, ClockTime, Math::Vector3(0.0f, 1.0f, 0.0f));

		ENanosuit.GetComponent<Components::TransformComponent>().Get()->mTransform = NanosuitMatrix;

		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());

		SampleScene.Systems.Update_All(dt);
		
		Skybox.Render();
		states.EnabledDepth_DisabledStencil.Bind();

		Graphics::API::Context::PresentFrame();
	}
};