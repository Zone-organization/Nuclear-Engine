#pragma once
#include "Common.h"
#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"
#include "Sample4.h"
#include "Sample5.h"
#include "SponzaDemo.h"
#include "Playground.h"

class SampleSelector : public Core::Client
{
public:

	void OnWindowResize(int width, int height) override
	{
		Graphics::Context::GetInstance().GetSwapChain()->Resize(width, height);
	}
	SampleSelector()
	{
		//	Instance = this;
	}
	void StartSample(Client* sample)
	{
		ImGui::End();
		Core::Engine::GetInstance().EndFrame();
		Core::Engine::GetInstance().EndClient();

		Core::Engine::GetInstance().LoadClient(sample);
		Core::Engine::GetInstance().EndClient();
	}

	void Render(float dt) override
	{
		const float ClearColor[] = { 0.350f,  0.350f,  0.350f, 1.0f };

		auto* RTV = Graphics::Context::GetInstance().GetSwapChain()->GetCurrentBackBufferRTV();
		auto* DSV = Graphics::Context::GetInstance().GetSwapChain()->GetDepthBufferDSV();

		Graphics::Context::GetInstance().GetContext()->SetRenderTargets(1, &RTV, DSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetInstance().GetContext()->ClearRenderTarget(RTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		Graphics::Context::GetInstance().GetContext()->ClearDepthStencil(DSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		using namespace Graphics;
		ImGui::Begin("SampleSelector");

		ImGui::Text("Select Sample");

		if (ImGui::Button("Sample1 : Basic model loading & animation & rendering"))
		{
			Sample1 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample2 : Introduction to rendering pipelines and shading models."))
		{
			Sample2 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample3 : PhysX & Scripting Test WIP"))             //Improve
		{
			Sample3 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample4 : Multiple Cameras"))
		{
			Sample4 demo;
			return StartSample(&demo);
		}
		if (ImGui::Button("Sample5 : Async Asset Importing"))
		{
			Sample5 demo;
			return StartSample(&demo);
		}

		ImGui::Text("Select Demo");

		if (ImGui::Button("Sponza Demo"))
		{
			SponzaDemo demo;
			return StartSample(&demo);
		}

		if (ImGui::Button("Playground"))
		{
		Playground demo;
		return StartSample(&demo);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
};