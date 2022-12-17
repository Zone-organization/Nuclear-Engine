#pragma once
#include <NE_Common.h>
#include <ECS/Common.h>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <memory>
#include <Interfaces/NonCopyable.h>

namespace Nuclear
{
	namespace ECS
	{

		class NEAPI BaseSystem : Interfaces::NonCopyable {
		public:
			typedef size_t Family;

			virtual ~BaseSystem();

			virtual void Load() { }

			virtual void Update(TimeDelta dt) = 0;

			virtual void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight) { }

			static Family family_counter_;

		protected:
		};


		template <typename Derived>
		class System : public BaseSystem {
		public:
			virtual ~System() {}

		private:

			friend class SystemManager;

			static Family family() {
				static Family family = family_counter_++;
				return family;
			}
		};

		class NEAPI SystemManager : Interfaces::NonCopyable {
		public:
			SystemManager() {}

			template <typename S>
			void Add(std::shared_ptr<S> system) {
				systems_.insert(std::make_pair(S::family(), system));
			}

			template <typename S, typename ... Args>
			std::shared_ptr<S> Add(Args && ... args) {
				std::shared_ptr<S> s(new S(std::forward<Args>(args) ...));
				Add(s);
				return s;
			}


			template <typename S>
			std::shared_ptr<S> GetSystem() {
				auto it = systems_.find(S::family());
				//assert(it != systems_.end());
				return it == systems_.end()
					? std::shared_ptr<S>()
					: std::shared_ptr<S>(std::static_pointer_cast<S>(it->second));
			}


			template <typename S>
			void Update(ECS::TimeDelta dt) {
				std::shared_ptr<S> s = system<S>();
				s->Update(dt);
			}

			void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight);

			void Update_All(ECS::TimeDelta dt);


			//void Configure();

		private:
			//bool initialized_ = false;
			std::unordered_map<BaseSystem::Family, std::shared_ptr<BaseSystem>> systems_;
		};
	}
}