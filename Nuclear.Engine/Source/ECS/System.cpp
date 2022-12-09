#include <ECS\System.h>

namespace Nuclear
{
	namespace ECS
	{
		BaseSystem::Family BaseSystem::family_counter_;

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