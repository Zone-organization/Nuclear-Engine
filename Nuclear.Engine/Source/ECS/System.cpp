#include <ECS\System.h>

namespace Nuclear
{
	namespace ECS
	{
		BaseSystem::Family BaseSystem::family_counter_;

		void SystemManager::ResizeRTs(Uint32 RTWidth, Uint32 RTHeight)
		{
			for (auto& pair : systems_) {
				pair.second->ResizeRTs(RTWidth, RTHeight);
			}
		}

		void SystemManager::Update_All(ECS::TimeDelta dt) {
			for (auto& pair : systems_) {
				pair.second->Update(dt);
			}
		}

		BaseSystem::~BaseSystem()
		{
		}

		//void SystemManager::Configure() {
		//	for (auto& pair : systems_) {
		//		pair.second->Configure();
		//	}
		//	initialized_ = true;
		//}
	}
}