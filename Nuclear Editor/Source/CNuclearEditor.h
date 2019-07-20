#pragma once
#include "Common.h"

void ViewMaterialInfo(Assets::Material* material, Managers::AssetManager* Manager)
{
	using namespace Graphics;
	std::string name = Manager->mHashedMaterialsPaths[material->GetName()].mInputPath + Utilities::int_to_hex<Uint32>(material->GetName());

	if (Manager)
		name = Manager->mHashedMaterialsPaths[material->GetName()].mInputPath + Utilities::int_to_hex<Uint32>(material->GetName());
	else
		name = Utilities::int_to_hex<Uint32>(material->GetName()).c_str();

	ImGui::Begin(name.c_str());

	for (int i = 0; i < material->mPixelShaderTextures.size(); i++)
	{
		ImGui::Text(std::string("TextureSet Index: " + std::to_string(i)).c_str());
		for (int j = 0; j < material->mPixelShaderTextures.at(i).mData.size(); j++)
		{
			ImGui::Image(&material->mPixelShaderTextures.at(i).mData.at(j).mTex, ImVec2(128, 128));
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}
	ImGui::End();
}
class CNuclearEditor : public Core::Game
{

	void Load()
	{


	}
	void OnMouseMovement(int xpos_a, int ypos_a) override
	{

	}
	void Update(float deltatime) override
	{
		
	}


	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetSwapChain()->Resize(width, height);
	}

	void Render(float dt) override
	{
		// Clear the back buffer 
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };
		Graphics::Context::GetContext()->ClearRenderTarget(nullptr, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetContext()->ClearDepthStencil(nullptr, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		{			
			using namespace Graphics;
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
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
	}
};