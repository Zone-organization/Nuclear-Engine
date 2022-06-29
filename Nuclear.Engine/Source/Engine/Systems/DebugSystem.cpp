#include <Engine\Systems\DebugSystem.h>
#include <Engine/Graphics/ImGui.h>

namespace Nuclear
{
	namespace Systems
	{
		DebugSystem::DebugSystem()
		{
		}
		DebugSystem::~DebugSystem()
		{
		}
		void DebugSystem::Update(ECS::TimeDelta dt)
		{
			mRegisteredRTs.clear();
		}
		void DebugSystem::ShowRendertargets()
		{	
			ImGui::Begin("DebugSystem - Registered RenderTargets");

			std::vector<const char*> rt_gr;

			std::set<std::string> RT_Groups;
			for (auto& i : mRegisteredRTs)
			{
				RT_Groups.insert(i->GetDesc().mType);
			}
			for (auto& i : RT_Groups)
			{
				rt_gr.push_back(i.c_str());
			}

			static int item_current = 0;
			ImGui::Combo("RenderTarget Type", &item_current, rt_gr.data(), rt_gr.size());

			for(Uint32 i = 0; i < mRegisteredRTs.size(); i++)
			{
				auto& ref = mRegisteredRTs.at(i);
				if(ref->GetDesc().mType == rt_gr.at(item_current))
				{
					//if (i % 2 == 0)
					//{
					//	ImGui::Image(ref->GetSRV(), { 256.f,256.f });
					//	//ImGui::SameLine();
					//	ImGui::Text(ref->GetDesc().mName.c_str());

					////	ImGui::SameLine();
					//}
					//else {
					//	ImGui::SameLine();
					//	ImGui::Image(ref->GetSRV(), { 256.f,256.f });
					//	ImGui::SameLine();
					//	ImGui::Text(ref->GetDesc().mName.c_str());
					//}

				}
			}

			ImGui::End();
		
		}
	}
}