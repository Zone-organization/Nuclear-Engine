#pragma once
#include "Common.h"
class Sample1 : public Core::Game
{
	std::shared_ptr<Systems::RenderSystem> Renderer;
	std::shared_ptr<LLGL::Input> Input;

	//Assets
	Assets::Texture DiffuseTex;
	Assets::Texture SpecularTex;
	Assets::Texture WhiteTex;
	Assets::Texture GreyTex;
	Assets::Texture GridTex;

	Assets::Mesh CubeAsset;
	Assets::Mesh SphereAsset;
	Assets::Mesh NanosuitAsset;
	Assets::Mesh GridAsset;

	Assets::Material CubeMaterial;
	Assets::Material SphereMaterial;
	Assets::Material NanosuitMaterial;
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
		//Create some shapes
		Assets::Mesh::CreateCube(&CubeAsset);
		CubeAsset.Initialize(Renderer->GetVertexShader());

		Assets::Mesh::CreateSphere(&SphereAsset);
		SphereAsset.Initialize(Renderer->GetVertexShader());

		Assets::Mesh::CreateGrid(&GridAsset, Assets::MeshVertexDesc(), 6, 6, 10, 10);
		GridAsset.Initialize(Renderer->GetVertexShader());

		//NanoSuit Creation
		Managers::MeshLoadingDesc ModelDesc;
		ModelDesc.LoadDiffuseTextures = true;
		ModelDesc.LoadSpecularTextures = true;
		NanosuitAsset = Managers::AssetManager::Import("Assets/Common/Models/CrytekNanosuit/nanosuit.obj", ModelDesc, NanosuitMaterial);
		NanosuitAsset.Initialize(Renderer->GetVertexShader());

		//Load some textures manually
		DiffuseTex = Managers::AssetManager::Import("Assets/Common/Textures/crate_diffuse.png", Assets::TextureUsageType::Diffuse);
		SpecularTex = Managers::AssetManager::Import("Assets/Common/Textures/crate_specular.png", Assets::TextureUsageType::Specular);
		WhiteTex = Managers::AssetManager::Import("Assets/Common/Textures/white.png");
		GreyTex = Managers::AssetManager::Import("Assets/Common/Textures/grey.png");
		GridTex = Managers::AssetManager::Import("Assets/Common/Textures/Grid.png");

		//TODO::MATERIAL

		//Initialize Materials
		/*Assets::TextureSet CubeSet;
		CubeSet.push_back(DiffuseTex);
		CubeSet.push_back(SpecularTex);
		CubeMaterial.mPixelShaderTextures.push_back(CubeSet);

		Assets::TextureSet SphereTextures;
		WhiteTex.SetUsageType(Assets::TextureUsageType::Diffuse);
		SphereTextures.push_back(WhiteTex);
		WhiteTex.SetUsageType(Assets::TextureUsageType::Specular);
		SphereTextures.push_back(WhiteTex);
		SphereMaterial.mPixelShaderTextures.push_back(SphereTextures);


		Assets::TextureSet GridTextures;
		GridTex.SetUsageType(Assets::TextureUsageType::Diffuse);
		GridTextures.push_back(GridTex);
		GreyTex.SetUsageType(Assets::TextureUsageType::Specular);
		GridTextures.push_back(GreyTex);
		GridMaterial.mPixelShaderTextures.push_back(GridTextures);

		GridMaterial.SetPixelShader(Renderer->GetPixelShader());
		CubeMaterial.SetPixelShader(Renderer->GetPixelShader());
		SphereMaterial.SetPixelShader(Renderer->GetPixelShader());
		NanosuitMaterial.SetPixelShader(Renderer->GetPixelShader());*/

		//CubeSet.clear();
		//SphereTextures.clear();
		//GridTextures.clear();

		CubeMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		CubeMaterial.SetMaterialVariable("Shininess", 64.0f);

		SphereMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		SphereMaterial.SetMaterialVariable("Shininess", 64.0f);

		NanosuitMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		NanosuitMaterial.SetMaterialVariable("Shininess", 64.0f);

		GridMaterial.SetMaterialVariable("ModelColor", Math::Vector3(1.0f, 1.0f, 1.0f));
		GridMaterial.SetMaterialVariable("Shininess", 64.0f);

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
		ECube = SampleScene.CreateEntity();
		ELamp = SampleScene.CreateEntity();
		EGrid = SampleScene.CreateEntity();
		ENanosuit = SampleScene.CreateEntity();

		//Assign Components
		ECube.Assign<Components::MeshComponent>(&CubeAsset, &CubeMaterial, true);
		ELamp.Assign<Components::MeshComponent>(&SphereAsset, &SphereMaterial, true);
		EGrid.Assign<Components::MeshComponent>(&GridAsset, &GridMaterial);
		ENanosuit.Assign<Components::MeshComponent>(&NanosuitAsset, &NanosuitMaterial);

	}
	void Load()
	{
		//Setup Input System
		// Add input event listener to window
		Input = std::make_shared<LLGL::Input>();
		Core::Application::GetWindow()->AddEventListener(Input);

		Systems::RenderSystemDesc desc;
		Renderer = SampleScene.Systems.Add<Systems::RenderSystem>(desc, &Camera);
		SampleScene.Systems.Configure();

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

		Managers::AssetManager::mSaveTextureNames = true;

		SetupLights();

		SetupAssets();

		SetupEntities();
		
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
		if (Input->KeyPressed(Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Input->KeyPressed(Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Input->KeyPressed(Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Input->KeyPressed(Input::Keyboard::Key::D))
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		Camera.Update();
	}


	void Render(float dt) override
	{  
		// Set the render context as the initial render target
		Graphics::Context::GetCommands()->BeginRenderPass(*Graphics::Context::GetRenderContext());
		{
			// Clear color buffer
			Graphics::Context::GetCommands()->Clear(LLGL::ClearFlags::ColorDepth);
			Graphics::Context::GetCommands()->SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Graphics::Context::GetCommands()->SetGraphicsPipeline(*Renderer->Pipeline);

			// Draw triangle with 3 vertices
			Renderer->InstantRender(ENanosuit.GetComponent<Components::MeshComponent>().Get());

		}
		Graphics::Context::GetCommands()->EndRenderPass();


		//ImGui::Render();
		//Graphics::ImGui_Renderer::RenderDrawData(ImGui::GetDrawData());
		Graphics::Context::PresentFrame();
	}
};
/*
//#include <Engine\Graphics\ImGui_Renderer.h>

class Sample1 : public Core::Game
{
protected:
	

public:
	Sample1()
	{
	}

	

	void Load()
	{
		
		states.EnabledDepth_DisabledStencil.Bind();
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);

		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	
	void MaterialEditor()
	{
		ImGui::Begin("Material Editor");                          // Create a window called "Hello, world!" and append into it.

		for (auto i : Managers::AssetManager::mImportedMaterials)
		{
			for (auto texset : i.second.mPixelShaderTextures)
			{
				for (auto tex : texset)
				{
					ImGui::Image(&tex.mTexture.mTexture, ImVec2(tex.mTexture.mTexture.GetDimensions().x, tex.mTexture.mTexture.GetDimensions().y));
				}
			}
		}

		ImGui::End();

	} 

	
	void Render(float dt) override
	{
		ImGui::NewFrame();

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

		ECube.GetComponent<Components::MeshComponent>().Get()->mMultiRenderTransforms = CubeTransforms;
		ELamp.GetComponent<Components::MeshComponent>().Get()->mMultiRenderTransforms = LampTransforms;
		
		Math::Matrix4 NanosuitMatrix(1.0f);
		NanosuitMatrix = Math::translate(NanosuitMatrix, Math::Vector3(5.0f, -1.75f, 0.0f));
		NanosuitMatrix = Math::scale(NanosuitMatrix, Math::Vector3(0.25f));
		NanosuitMatrix = Math::rotate(NanosuitMatrix, ClockTime, Math::Vector3(0.0f, 1.0f, 0.0f));

		ENanosuit.GetComponent<Components::TransformComponent>().Get()->mTransform = NanosuitMatrix;

		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Up))
			EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform = Math::translate(EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform, Math::Vector3(0.0f, 0.01f, 0.0f));
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Down))
			EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform = Math::translate(EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform, Math::Vector3(0.0f, -0.01f, 0.0f));
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Right))
			EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform = Math::translate(EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform, Math::Vector3(0.01f, 0.0f, 0.0f));
		if (Core::Input::Keyboard::isKeyPressed(Core::Input::Keyboard::Key::Left))
			EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform = Math::translate(EGrid.GetComponent<Components::TransformComponent>().Get()->mTransform, Math::Vector3(-0.01f, 0.0f, 0.0f));

		MaterialEditor();

		spotLight.SetPosition(Camera.GetPosition());
		spotLight.SetDirection(Camera.GetFrontView());

		SampleScene.Systems.Update_All(dt);
		
		Skybox.Render();
		states.EnabledDepth_DisabledStencil.Bind();


		//ImGui::Render();
		Graphics::ImGui_Renderer::RenderDrawData(ImGui::GetDrawData());
		Graphics::API::Context::PresentFrame();
	}
};
*/