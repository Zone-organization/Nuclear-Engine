#pragma once
#include "Common.h"

class GameObject
{
public:
	GameObject() {}
	Components::Model *model;
};

class RenderSystem : public Core::System<RenderSystem> {
public:

	void update(Core::EntityManager &es, Core::EventManager &events,Core::TimeDelta dt) override
	{
		es.each<GameObject>([this](Core::Entity entity, GameObject& obj)
		{
			obj.model->Draw();
		});
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
	Components::Model Cube;
	Components::Model Crate;

	Shading::Techniques::NoLight LightTech;
	Renderers::Renderer3D Renderer;

	GameObject object;
	GameObject child;

	float lastX = _Width_ / 2.0f;
	float lastY = _Height_ / 2.0f;
	bool firstMouse = true;

public:
	Playground()
		: entities(events), systems(entities, events)
	{
	}
	void Load()
	{
		systems.add<RenderSystem>();
		systems.configure();

		Camera.Initialize(Math::Perspective(Math::ToRadians(45.0f), Core::Application::GetAspectRatiof(), 0.1f, 100.0f));
		Renderer.SetCamera(&Camera);
		Renderer.SetTechnique(&LightTech);
		Renderer.Bake();

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
		Cube.Initialize(&Renderer.GetShader());

		AssetManager::CreateTextureFromFile("Assets/Common/Textures/white.png", &WhiteTex, Desc);

		Textures.clear();
		DiffuseTex.Texture = WhiteTex;
		Textures.push_back(DiffuseTex);

		Components::Model::CreateSphere(&Crate, Textures);
		Crate.Initialize(&Renderer.GetShader());

		object.model = &Cube;	

		Core::Entity entity = entities.create();
		entity.assign<GameObject>(object);

		//child.SetModel(&Crate);
		//object.AddChild(&child);
		
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
		Renderer.GetShader().Bind();
		states.DefaultSampler.PSBind(0);

		systems.update_all(deltatime);

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