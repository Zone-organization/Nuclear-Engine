#pragma once
#include "Common.h"

struct EventComponentReciever : public Core::Receiver<EventComponentReciever> {
	void receive(const Core::ComponentAddedEvent<Components::GameObject> &shit)
	{
		std::cout << "Working.\n";
	}

};
class Playground : public Core::Game
{
protected:

	Core::EventManager events;
	Core::EntityManager entities;
	Core::SystemManager systems;

	API::Texture WoodenBoxTex;
	API::Texture WhiteTex;

	Components::FlyCamera Camera;
	API::CommonStates states;
	Core::Entity entity;
	Components::Model Cube;
	Components::Model lamp;
	std::shared_ptr<Systems::RenderSystem> Renderer;
	Components::GameObject child;
	Components::GameObject object;

	Shading::Techniques::NoLight lighttech;
	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

	EventComponentReciever reciever;

public:
	Playground()
		: entities(events), systems(entities, events)
	{
	}
	void Load()
	{
		Systems::RenderSystemDesc desc;
		desc.Light_Rendering_Tech = &lighttech;
		Renderer = systems.Add<Systems::RenderSystem>(desc);
		
		systems.Configure();
		
		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));

		Renderer->SetCamera(&Camera);
		Renderer->Bake();

		API::Texture_Desc Desc;
		Desc.Format = API::Format::R8G8B8A8_UNORM;
		Desc.Type = API::TextureType::Texture2D;

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/woodenbox.jpg", &WoodenBoxTex, Desc);


		std::vector<Components::MeshTexture> Textures;
		Components::MeshTexture DiffuseTex;
		DiffuseTex.Texture = WoodenBoxTex;
		DiffuseTex.type = Components::MeshTextureType::Diffuse;
		Textures.push_back(DiffuseTex);

		Components::Model::CreateCube(&Cube, Textures);
		Cube.Initialize(&Renderer->GetShader());

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);

		Textures.clear();
		DiffuseTex.Texture = WhiteTex;
		Textures.push_back(DiffuseTex);

		Components::Model::CreateSphere(&lamp, Textures);
		lamp.Initialize(&Renderer->GetShader());

		object.SetModel(&lamp);
		child.SetModel(&lamp);

		events.Subscribe<Core::ComponentAddedEvent<Components::GameObject>>(reciever);


		entity = entities.Create();

		entity.Assign<Components::GameObject>(object);
		systems.Update_All(0.0f);

		//object.SetModel(&lamp);
		ImGui::StyleColorsDark();

		states.EnabledDepth_DisabledStencil.Bind();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
		Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
		Core::Application::Display();
	}

	void OnMouseMovement(double xpos_a, double ypos_a) override
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
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		Camera.ProcessEye(xoffset, yoffset);
	}
	void Update(float deltatime) override
	{
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::W))
			Camera.ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::A))
			Camera.ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::S))
			Camera.ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::D))
			Camera.ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);

		Camera.Update();

	}
	void ShowOverlay(bool show)
	{
		const float DISTANCE = 10.0f;
		static int corner = 0;
		ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f)); // Transparent background
		if (ImGui::Begin("FPS Overlay", &show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Text("FPS Overlay\n");
			ImGui::Separator();
			ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

		}
		ImGui::PopStyleColor();
	}
	void Render(float deltatime) override
	{
		Core::Context::Begin();
		ImGui::NE_NewFrame();
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);
		

		Math::Matrix4 CubeMatrix;
		//CubeMatrix = Math::Translate(CubeMatrix, Math::Vector3(3.0f, 0.0f, 0.0f));
		//Camera.SetModelMatrix(CubeMatrix);
		Renderer->GetShader().Bind();
		states.DefaultSampler.PSBind(0);

		//object.GetModel()->Draw();
		systems.Update_All(deltatime);

		//object.Render();
		//Cube.Draw();

		ShowOverlay(true);

		ImGui::Render();
		Core::Context::End();
	}
	void Shutdown() override
	{
		API::Texture::Delete(&WoodenBoxTex);
	}
};