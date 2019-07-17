/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#pragma once


#include <cstdint>
#include <tuple>
#include <new>
#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>
#include <functional>
#include <Base\NE_Common.h>
#include "Base/Utilities/Pool.h"
#include "ECSConfig.h"
#include "Event.h"
#include "Base/Utilities/NonCopyable.h"

namespace NuclearEngine {
	namespace ECS {

		typedef std::uint32_t uint32_t;
		typedef std::uint64_t uint64_t;

		class EntityManager;


		template <typename C, typename EM = EntityManager>
		class ComponentHandle;



		/** A convenience handle around an Entity::Id.
		 *
		 * If an entity is Destroyed, any copies will be InValidated. Use Valid() to
		 * check for Validity before using.
		 *
		 * Create entities with `EntityManager`:
		 *
		 *     Entity entity = entity_manager->Create();
		 */
		class NEAPI Entity {
		public:
			struct Id {
				Id() : id_(0) {}
				explicit Id(uint64_t id) : id_(id) {}
				Id(uint32_t index, uint32_t version) : id_(uint64_t(index) | uint64_t(version) << 32UL) {}

				uint64_t id() const { return id_; }

				bool operator == (const Id &other) const { return id_ == other.id_; }
				bool operator != (const Id &other) const { return id_ != other.id_; }
				bool operator < (const Id &other) const { return id_ < other.id_; }

				uint32_t index() const { return id_ & 0xffffffffUL; }
				uint32_t version() const { return id_ >> 32; }
				

			private:
				friend class EntityManager;
				uint64_t id_;
			};


			/**
			 * Id of an inValid Entity.
			 */
			static const Id INVALID;

			Entity() = default;
			Entity(EntityManager *manager, Entity::Id id) : manager_(manager), id_(id) {}
			Entity(const Entity &other) = default;
			Entity &operator = (const Entity &other) = default;

			/**
			 * Check if Entity handle is inValid.
			 */
			operator bool() const {
				return Valid();
			}

			bool operator == (const Entity &other) const {
				return other.manager_ == manager_ && other.id_ == id_;
			}

			bool operator != (const Entity &other) const {
				return !(other == *this);
			}

			bool operator < (const Entity &other) const {
				return other.id_ < id_;
			}

			/**
			 * Is this Entity handle Valid?
			 *
			 * In older versions of EntityX, there were no guarantees around entity
			 * Validity if a previously allocated entity slot was reAssigned. That is no
			 * longer the case: if a slot is reAssigned, old Entity::Id's will be
			 * inValid.
			 */
			bool Valid() const;

			/**
			 * InValidate Entity handle, disassociating it from an EntityManager and inValidating its ID.
			 *
			 * Note that this does *not* affect the underlying entity and its
			 * components. Use Destroy() to Destroy the associated Entity and components.
			 */
			void InValidate();

			Id id() const { return id_; }

			template <typename C, typename ... Args>
			ComponentHandle<C> Assign(Args && ... args);

			template <typename C>
			ComponentHandle<C> Assign_from_copy(const C &component);

			template <typename C, typename ... Args>
			ComponentHandle<C> Replace(Args && ... args);

			template <typename C>
			void Remove();

			template <typename C, typename = typename std::enable_if<!std::is_const<C>::value>::type>
			ComponentHandle<C> GetComponent();

			template <typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
			const ComponentHandle<C, const EntityManager> GetComponent() const;

			template <typename ... Components>
			std::tuple<ComponentHandle<Components>...> GetComponents();

			template <typename ... Components>
			std::tuple<ComponentHandle<const Components, const EntityManager>...> GetComponents() const;

			template <typename C>
			bool HasComponent() const;

			template <typename A, typename ... Args>
			void unpack(ComponentHandle<A> &a, ComponentHandle<Args> & ... args);

			/**
			 * Destroy and InValidate this Entity.
			 */
			void Destroy();

			std::bitset<MAX_COMPONENTS> component_mask() const;

		private:
			EntityManager * manager_ = nullptr;
			Entity::Id id_ = INVALID;
		};


		/**
		 * A ComponentHandle<C> is a wrapper around an instance of a component.
		 *
		 * It provides safe access to components. The handle will be InValidated under
		 * the following conditions:
		 *
		 * - If a component is Removed from its host entity.
		 * - If its host entity is Destroyed.
		 */
		template <typename C, typename EM>
		class ComponentHandle {
		public:
			typedef C ComponentType;

			ComponentHandle() : manager_(nullptr) {}

			bool Valid() const;
			operator bool() const;

			C *operator -> ();
			const C *operator -> () const;

			C &operator * ();
			const C &operator * () const;

			C *Get();
			const C *Get() const;

			/**
			 * Remove the component from its entity and Destroy it.
			 */
			void Remove();

			/**
			 * Returns the Entity associated with the component
			 */
			Entity GetEntity();

			bool operator == (const ComponentHandle<C> &other) const {
				return manager_ == other.manager_ && id_ == other.id_;
			}

			bool operator != (const ComponentHandle<C> &other) const {
				return !(*this == other);
			}

		private:
			friend class EntityManager;

			ComponentHandle(EM *manager, Entity::Id id) :
				manager_(manager), id_(id) {}

			EM *manager_;
			Entity::Id id_;
		};


		/**
		 * Base component class, only used for insertion into collections.
		 *
		 * Family is used for registration.
		 */
		struct NEAPI BaseComponent {
		public:
			typedef size_t Family;

			// NOTE: Component memory is *always* managed by the EntityManager.
			// Use Entity::Destroy() instead.
			void operator delete(void *p) { fail(); }
			void operator delete[](void *p) { fail(); }


		protected:
			static void fail() {
#if defined(_HAS_EXCEPTIONS) || defined(__EXCEPTIONS)
				throw std::bad_alloc();
#else
				std::abort();
#endif
			}

			static Family family_counter_;
		};


		/**
		 * Component implementations should inherit from this.
		 *
		 * Components MUST provide a no-argument constructor.
		 * Components SHOULD provide convenience constructors for initializing on Assignment to an Entity::Id.
		 *
		 * This is a struct to imply that components should be data-only.
		 *
		 * Usage:
		 *
		 *     struct Position : public Component<Position> {
		 *       Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
		 *
		 *       float x, y;
		 *     };
		 *
		 * family() is used for registration.
		 */
		template <typename Derived>
		struct Component : public BaseComponent {
		public:
			typedef ComponentHandle<Derived> Handle;
			typedef ComponentHandle<const Derived, const EntityManager> ConstHandle;

		private:
			friend class EntityManager;
			/// Used internally for registration.
			static Family family();
		};


		/**
		 * Emitted when an entity is Added to the system.
		 */
		struct EntityCreatedEvent : public Event<EntityCreatedEvent> {
			explicit EntityCreatedEvent(Entity entity) : entity(entity) {}
			virtual ~EntityCreatedEvent(){}

			Entity entity;
		};


		/**
		 * Called just prior to an entity being Destroyed.
		 */
		struct EntityDestroyedEvent : public Event<EntityDestroyedEvent> {
			explicit EntityDestroyedEvent(Entity entity) : entity(entity) {}
			virtual ~EntityDestroyedEvent(){}

			Entity entity;
		};


		/**
		 * Emitted when any component is Added to an entity.
		 */
		template <typename C>
		struct ComponentAddedEvent : public Event<ComponentAddedEvent<C>> {
			ComponentAddedEvent(Entity entity, ComponentHandle<C> component) :
				entity(entity), component(component) {}

			Entity entity;
			ComponentHandle<C> component;
		};

		/**
		 * Emitted when any component is Removed from an entity.
		 */
		template <typename C>
		struct ComponentRemovedEvent : public Event<ComponentRemovedEvent<C>> {
			ComponentRemovedEvent(Entity entity, ComponentHandle<C> component) :
				entity(entity), component(component) {}

			Entity entity;
			ComponentHandle<C> component;
		};

		/**
		 * Helper class to perform component operations in a typed manner.
		 */
		class BaseComponentHelper {
		public:
			virtual ~BaseComponentHelper() {}
			virtual void Remove_component(Entity e) = 0;
			virtual void copy_component_to(Entity source, Entity tarGet) = 0;
		};

		template <typename C>
		class ComponentHelper : public BaseComponentHelper {
		public:
			void Remove_component(Entity e) override {
				e.Remove<C>();
			}
			void copy_component_to(Entity source, Entity tarGet) override {
				tarGet.Assign_from_copy<C>(*(source.GetComponent<C>().Get()));
			}
		};

		/**
		 * Manages Entity::Id creation and component Assignment.
		 */
		class NEAPI EntityManager : Utilities::NonCopyable {
		public:
			typedef std::bitset<MAX_COMPONENTS> ComponentMask;

			explicit EntityManager(EventManager &event_manager);
			virtual ~EntityManager();

			/// An iterator over a view of the entities in an EntityManager.
			/// If All is true it will iterate over all Valid entities and will ignore the entity mask.
			template <class Delegate, bool All = false>
			class ViewIterator : public std::iterator<std::input_iterator_tag, Entity::Id> {
			public:
				Delegate & operator ++() {
					++i_;
					next();
					return *static_cast<Delegate*>(this);
				}
				bool operator == (const Delegate& rhs) const { return i_ == rhs.i_; }
				bool operator != (const Delegate& rhs) const { return i_ != rhs.i_; }
				Entity operator * () { return Entity(manager_, manager_->Create_id(i_)); }
				const Entity operator * () const { return Entity(manager_, manager_->Create_id(i_)); }

			protected:
				ViewIterator(EntityManager *manager, uint32_t index)
					: manager_(manager), i_(index), capacity_(manager_->capacity()), free_cursor_(~0UL) {
					if (All) {
						std::sort(manager_->free_list_.begin(), manager_->free_list_.end());
						free_cursor_ = 0;
					}
				}
				ViewIterator(EntityManager *manager, const ComponentMask mask, uint32_t index)
					: manager_(manager), mask_(mask), i_(index), capacity_(manager_->capacity()), free_cursor_(~0UL) {
					if (All) {
						std::sort(manager_->free_list_.begin(), manager_->free_list_.end());
						free_cursor_ = 0;
					}
				}

				void next() {
					while (i_ < capacity_ && !predicate()) {
						++i_;
					}

					if (i_ < capacity_) {
						Entity entity = manager_->Get(manager_->Create_id(i_));
						static_cast<Delegate*>(this)->next_entity(entity);
					}
				}

				inline bool predicate() {
					return (All && Valid_entity()) || (manager_->entity_component_mask_[i_] & mask_) == mask_;
				}

				inline bool Valid_entity() {
					const std::vector<uint32_t> &free_list = manager_->free_list_;
					if (free_cursor_ < free_list.size() && free_list[free_cursor_] == i_) {
						++free_cursor_;
						return false;
					}
					return true;
				}

				EntityManager *manager_;
				ComponentMask mask_;
				uint32_t i_;
				size_t capacity_;
				size_t free_cursor_;
			};

			template <bool All>
			class BaseView {
			public:
				class Iterator : public ViewIterator<Iterator, All> {
				public:
					Iterator(EntityManager *manager,
						const ComponentMask mask,
						uint32_t index) : ViewIterator<Iterator, All>(manager, mask, index) {
						ViewIterator<Iterator, All>::next();
					}

					void next_entity(Entity &entity) {}
				};

				Iterator begin() { return Iterator(manager_, mask_, 0); }
				Iterator end() { return Iterator(manager_, mask_, uint32_t(manager_->capacity())); }
				const Iterator begin() const { return Iterator(manager_, mask_, 0); }
				const Iterator end() const { return Iterator(manager_, mask_, manager_->capacity()); }

			private:
				friend class EntityManager;

				explicit BaseView(EntityManager *manager) : manager_(manager) { mask_.set(); }
				BaseView(EntityManager *manager, ComponentMask mask) :
					manager_(manager), mask_(mask) {}

				EntityManager *manager_;
				ComponentMask mask_;
			};

			template <bool All, typename ... Components>
			class TypedView : public BaseView<All> {
			public:
				template <typename T> struct identity { typedef T type; };

				void each(typename identity<std::function<void(Entity entity, Components&...)>>::type f) {
					for (auto it : *this)
						f(it, *(it.template GetComponent<Components>().Get())...);
				}

			private:
				friend class EntityManager;

				explicit TypedView(EntityManager *manager) : BaseView<All>(manager) {}
				TypedView(EntityManager *manager, ComponentMask mask) : BaseView<All>(manager, mask) {}
			};

			template <typename ... Components> using View = TypedView<false, Components...>;
			typedef BaseView<true> DebugView;

			template <typename ... Components>
			class UnpackingView {
			public:
				struct Unpacker {
					explicit Unpacker(ComponentHandle<Components> & ... handles) :
						handles(std::tuple<ComponentHandle<Components> & ...>(handles...)) {}

					void unpack(Entity &entity) const {
						unpack_<0, Components...>(entity);
					}


				private:
					template <int N, typename C>
					void unpack_(Entity &entity) const {
						std::get<N>(handles) = entity.GetComponent<C>();
					}

					template <int N, typename C0, typename C1, typename ... Cn>
					void unpack_(Entity &entity) const {
						std::get<N>(handles) = entity.GetComponent<C0>();
						unpack_<N + 1, C1, Cn...>(entity);
					}

					std::tuple<ComponentHandle<Components> & ...> handles;
				};


				class Iterator : public ViewIterator<Iterator> {
				public:
					Iterator(EntityManager *manager,
						const ComponentMask mask,
						uint32_t index,
						const Unpacker &unpacker) : ViewIterator<Iterator>(manager, mask, index), unpacker_(unpacker) {
						ViewIterator<Iterator>::next();
					}

					void next_entity(Entity &entity) {
						unpacker_.unpack(entity);
					}

				private:
					const Unpacker &unpacker_;
				};


				Iterator begin() { return Iterator(manager_, mask_, 0, unpacker_); }
				Iterator end() { return Iterator(manager_, mask_, static_cast<uint32_t>(manager_->capacity()), unpacker_); }
				const Iterator begin() const { return Iterator(manager_, mask_, 0, unpacker_); }
				const Iterator end() const { return Iterator(manager_, mask_, static_cast<uint32_t>(manager_->capacity()), unpacker_); }


			private:
				friend class EntityManager;

				UnpackingView(EntityManager *manager, ComponentMask mask, ComponentHandle<Components> & ... handles) :
					manager_(manager), mask_(mask), unpacker_(handles...) {}

				EntityManager *manager_;
				ComponentMask mask_;
				Unpacker unpacker_;
			};

			/**
			 * Number of managed entities.
			 */
			size_t size() const { return entity_component_mask_.size() - free_list_.size(); }

			/**
			 * Current entity capacity.
			 */
			size_t capacity() const { return entity_component_mask_.size(); }

			/**
			 * Return true if the given entity ID is still Valid.
			 */
			bool Valid(Entity::Id id) const {
				return id.index() < entity_version_.size() && entity_version_[id.index()] == id.version();
			}

			/**
			 * Create a new Entity::Id.
			 *
			 * Emits EntityCreatedEvent.
			 */
			Entity Create() {
				uint32_t index, version;
				if (free_list_.empty()) {
					index = index_counter_++;
					accomodate_entity(index);
					version = entity_version_[index] = 1;
				}
				else {
					index = free_list_.back();
					free_list_.pop_back();
					version = entity_version_[index];
				}

				Entity entity(this, Entity::Id(index, version));
				event_manager_.Emit<EntityCreatedEvent>(entity);
				return entity;
			}

			/**
			 * Create a new Entity by copying another. Copy-constructs each component.
			 *
			 * Emits EntityCreatedEvent.
			 */
			Entity Create_from_copy(Entity original) {
				assert(original.Valid());
				auto clone = Create();
				auto mask = original.component_mask();
				for (size_t i = 0; i < component_helpers_.size(); i++) {
					BaseComponentHelper *helper = component_helpers_[i];
					if (helper && mask.test(i))
						helper->copy_component_to(original, clone);
				}
				return clone;
			}


			/**
			 * Destroy an existing Entity::Id and its associated Components.
			 *
			 * Emits EntityDestroyedEvent.
			 */
			void Destroy(Entity::Id entity) {
				assert_Valid(entity);
				uint32_t index = entity.index();
				auto mask = entity_component_mask_[index];
				for (size_t i = 0; i < component_helpers_.size(); i++) {
					BaseComponentHelper *helper = component_helpers_[i];
					if (helper && mask.test(i))
						helper->Remove_component(Entity(this, entity));
				}
				event_manager_.Emit<EntityDestroyedEvent>(Entity(this, entity));
				entity_component_mask_[index].reset();
				entity_version_[index]++;
				free_list_.push_back(index);
			}

			Entity Get(Entity::Id id) {
				assert_Valid(id);
				return Entity(this, id);
			}

			/**
			 * Create an Entity::Id for a slot.
			 *
			 * NOTE: Does *not* check for Validity, but the Entity::Id constructor will
			 * fail if the ID is inValid.
			 */
			Entity::Id Create_id(uint32_t index) const {
				return Entity::Id(index, entity_version_[index]);
			}

			/**
			 * Assign a Component to an Entity::Id, passing through Component constructor arguments.
			 *
			 *     Position &position = em.Assign<Position>(e, x, y);
			 *
			 * @returns Smart pointer to newly Created component.
			 */
			template <typename C, typename ... Args>
			ComponentHandle<C> Assign(Entity::Id id, Args && ... args) {
				assert_Valid(id);
				const BaseComponent::Family family = component_family<C>();
				assert(!entity_component_mask_[id.index()].test(family));

				// Placement new into the component pool.
				Utilities::Pool<C> *pool = accomodate_component<C>();
				::new(pool->get(id.index())) C(std::forward<Args>(args) ...);

				// Set the bit for this component.
				entity_component_mask_[id.index()].set(family);

				// Create and return handle.
				ComponentHandle<C> component(this, id);
				event_manager_.Emit<ComponentAddedEvent<C>>(Entity(this, id), component);
				return component;
			}

			/**
			 * Remove a Component from an Entity::Id
			 *
			 * Emits a ComponentRemovedEvent<C> event.
			 */
			template <typename C>
			void Remove(Entity::Id id) {
				assert_Valid(id);
				const BaseComponent::Family family = component_family<C>();
				const uint32_t index = id.index();

				// Find the pool for this component family.
				Utilities::BasePool *pool = component_pools_[family];
				ComponentHandle<C> component(this, id);
				event_manager_.Emit<ComponentRemovedEvent<C>>(Entity(this, id), component);

				// Remove component bit.
				entity_component_mask_[id.index()].reset(family);

				// Call destructor.
				pool->destroy(index);
			}

			/**
			 * Check if an Entity has a component.
			 */
			template <typename C>
			bool HasComponent(Entity::Id id) const {
				assert_Valid(id);
				size_t family = component_family<C>();
				// We don't bother checking the component mask, as we return a nullptr anyway.
				if (family >= component_pools_.size())
					return false;
				Utilities::BasePool *pool = component_pools_[family];
				if (!pool || !entity_component_mask_[id.index()][family])
					return false;
				return true;
			}

			/**
			 * Retrieve a Component Assigned to an Entity::Id.
			 *
			 * @returns Pointer to an instance of C, or nullptr if the Entity::Id does not have that Component.
			 */
			template <typename C, typename = typename std::enable_if<!std::is_const<C>::value>::type>
			ComponentHandle<C> GetComponent(Entity::Id id) {
				assert_Valid(id);
				size_t family = component_family<C>();
				// We don't bother checking the component mask, as we return a nullptr anyway.
				if (family >= component_pools_.size())
					return ComponentHandle<C>();
				Utilities::BasePool *pool = component_pools_[family];
				if (!pool || !entity_component_mask_[id.index()][family])
					return ComponentHandle<C>();
				return ComponentHandle<C>(this, id);
			}

			/**
			 * Retrieve a Component Assigned to an Entity::Id.
			 *
			 * @returns Component instance, or nullptr if the Entity::Id does not have that Component.
			 */
			template <typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
			const ComponentHandle<C, const EntityManager> GetComponent(Entity::Id id) const {
				assert_Valid(id);
				size_t family = component_family<C>();
				// We don't bother checking the component mask, as we return a nullptr anyway.
				if (family >= component_pools_.size())
					return ComponentHandle<C, const EntityManager>();
				Utilities::BasePool *pool = component_pools_[family];
				if (!pool || !entity_component_mask_[id.index()][family])
					return ComponentHandle<C, const EntityManager>();
				return ComponentHandle<C, const EntityManager>(this, id);
			}

			template <typename ... Components>
			std::tuple<ComponentHandle<Components>...> GetComponents(Entity::Id id) {
				return std::make_tuple(GetComponent<Components>(id)...);
			}

			template <typename ... Components>
			std::tuple<ComponentHandle<const Components, const EntityManager>...> GetComponents(Entity::Id id) const {
				return std::make_tuple(GetComponent<const Components>(id)...);
			}

			/**
			 * Find Entities that have all of the specified Components.
			 *
			 * @code
			 * for (Entity entity : entity_manager.entities_with_components<Position, Direction>()) {
			 *   ComponentHandle<Position> position = entity.GetComponent<Position>();
			 *   ComponentHandle<Direction> direction = entity.GetComponent<Direction>();
			 *
			 *   ...
			 * }
			 * @endcode
			 */
			template <typename ... Components>
			View<Components...> entities_with_components() {
				auto mask = component_mask<Components ...>();
				return View<Components...>(this, mask);
			}

			template <typename T> struct identity { typedef T type; };

			template <typename ... Components>
			void each(typename identity<std::function<void(Entity entity, Components&...)>>::type f) {
				return entities_with_components<Components...>().each(f);
			}

			/**
			 * Find Entities that have all of the specified Components and Assign them
			 * to the given parameters.
			 *
			 * @code
			 * ComponentHandle<Position> position;
			 * ComponentHandle<Direction> direction;
			 * for (Entity entity : entity_manager.entities_with_components(position, direction)) {
			 *   // Use position and component here.
			 * }
			 * @endcode
			 */
			template <typename ... Components>
			UnpackingView<Components...> entities_with_components(ComponentHandle<Components> & ... components) {
				auto mask = component_mask<Components...>();
				return UnpackingView<Components...>(this, mask, components...);
			}

			/**
			 * Iterate over all *Valid* entities (ie. not in the free list). Not fast,
			 * so should only be used for debugging.
			 *
			 * @code
			 * for (Entity entity : entity_manager.entities_for_debugging()) {}
			 *
			 * @return An iterator view over all Valid entities.
			 */
			DebugView entities_for_debugging() {
				return DebugView(this);
			}

			template <typename C>
			void unpack(Entity::Id id, ComponentHandle<C> &a) {
				assert_Valid(id);
				a = this->component<C>(id);
			}

			/**
			 * Unpack components directly into pointers.
			 *
			 * Components missing from the entity will be set to nullptr.
			 *
			 * Useful for fast bulk iterations.
			 *
			 * ComponentHandle<Position> p;
			 * ComponentHandle<Direction> d;
			 * unpack<Position, Direction>(e, p, d);
			 */
			template <typename A, typename ... Args>
			void unpack(Entity::Id id, ComponentHandle<A> &a, ComponentHandle<Args> & ... args) {
				assert_Valid(id);
				a = this->component<A>(id);
				unpack<Args ...>(id, args ...);
			}

			/**
			 * Destroy all entities and reset the EntityManager.
			 */
			void reset();

			// Retrieve the component family for a type.
			template <typename C>
			static BaseComponent::Family component_family() {
				return Component<typename std::remove_const<C>::type>::family();
			}

		private:
			friend class Entity;
			template <typename C, typename EM>
			friend class ComponentHandle;


			inline void assert_Valid(Entity::Id id) const {
				assert(id.index() < entity_component_mask_.size() && "Entity::Id ID outside entity vector range");
				assert(entity_version_[id.index()] == id.version() && "Attempt to access Entity via a stale Entity::Id");
			}

			template <typename C>
			C *Get_component_ptr(Entity::Id id) {
				assert(Valid(id));
				Utilities::BasePool *pool = component_pools_[component_family<C>()];
				assert(pool);
				return static_cast<C*>(pool->get(id.index()));
			}

			template <typename C>
			const C *Get_component_ptr(Entity::Id id) const {
				assert_Valid(id);
				Utilities::BasePool *pool = component_pools_[component_family<C>()];
				assert(pool);
				return static_cast<const C*>(pool->get(id.index()));
			}

			ComponentMask component_mask(Entity::Id id) {
				assert_Valid(id);
				return entity_component_mask_.at(id.index());
			}

			template <typename C>
			ComponentMask component_mask() {
				ComponentMask mask;
				mask.set(component_family<C>());
				return mask;
			}

			template <typename C1, typename C2, typename ... Components>
			ComponentMask component_mask() {
				return component_mask<C1>() | component_mask<C2, Components ...>();
			}

			template <typename C>
			ComponentMask component_mask(const ComponentHandle<C> &c) {
				return component_mask<C>();
			}

			template <typename C1, typename ... Components>
			ComponentMask component_mask(const ComponentHandle<C1> &c1, const ComponentHandle<Components> &... args) {
				return component_mask<C1, Components ...>();
			}

			inline void accomodate_entity(uint32_t index) {
				if (entity_component_mask_.size() <= index) {
					entity_component_mask_.resize(index + 1);
					entity_version_.resize(index + 1);
					for (Utilities::BasePool *pool : component_pools_)
						if (pool) pool->expand(index + 1);
				}
			}

			template <typename C>
			Utilities::Pool<C> *accomodate_component() {
				BaseComponent::Family family = component_family<C>();
				if (component_pools_.size() <= family) {
					component_pools_.resize(family + 1, nullptr);
				}
				if (!component_pools_[family]) {
					Utilities::Pool<C> *pool = new Utilities::Pool<C>();
					pool->expand(index_counter_);
					component_pools_[family] = pool;
				}
				if (component_helpers_.size() <= family) {
					component_helpers_.resize(family + 1, nullptr);
				}
				if (!component_helpers_[family]) {
					ComponentHelper<C> *helper = new ComponentHelper<C>();
					component_helpers_[family] = helper;
				}
				return static_cast<Utilities::Pool<C>*>(component_pools_[family]);
			}


			uint32_t index_counter_ = 0;

			EventManager &event_manager_;
			// Each element in component_pools_ corresponds to a Utilities:: for a Component.
			// The index into the vector is the Component::family().
			std::vector<Utilities::BasePool*> component_pools_;
			// Each element in component_helpers_ corresponds to a ComponentHelper for a Component type.
			// The index into the vector is the Component::family().
			std::vector<BaseComponentHelper*> component_helpers_;
			// Bitmask of components associated with each entity. Index into the vector is the Entity::Id.
			std::vector<ComponentMask> entity_component_mask_;
			// Vector of entity version numbers. Incremented each time an entity is Destroyed
			std::vector<uint32_t> entity_version_;
			// List of available entity slots.
			std::vector<uint32_t> free_list_;
		};


		template <typename C>
		BaseComponent::Family Component<C>::family() {
			static Family family = family_counter_++;
			assert(family < MAX_COMPONENTS);
			return family;
		}


		template <typename C, typename ... Args>
		ComponentHandle<C> Entity::Assign(Args && ... args) {
			assert(Valid());
			return manager_->Assign<C>(id_, std::forward<Args>(args) ...);
		}

		template <typename C>
		ComponentHandle<C> Entity::Assign_from_copy(const C &component) {
			assert(Valid());
			return manager_->Assign<C>(id_, std::forward<const C &>(component));
		}

		template <typename C, typename ... Args>
		ComponentHandle<C> Entity::Replace(Args && ... args) {
			assert(Valid());
			auto handle = this->component<C>();
			if (handle) {
				*(handle.Get()) = C(std::forward<Args>(args) ...);
			}
			else {
				handle = manager_->Assign<C>(id_, std::forward<Args>(args) ...);
			}
			return handle;
		}

		template <typename C>
		void Entity::Remove() {
			assert(Valid() && HasComponent<C>());
			manager_->Remove<C>(id_);
		}

		template <typename C, typename>
		ComponentHandle<C> Entity::GetComponent() {
			assert(Valid());
			return manager_->GetComponent<C>(id_);
		}

		template <typename C, typename>
		const ComponentHandle<C, const EntityManager> Entity::GetComponent() const {
			assert(Valid());
			return const_cast<const EntityManager*>(manager_)->GetComponent<const C>(id_);
		}

		template <typename ... Components>
		std::tuple<ComponentHandle<Components>...> Entity::GetComponents() {
			assert(Valid());
			return manager_->GetComponents<Components...>(id_);
		}

		template <typename ... Components>
		std::tuple<ComponentHandle<const Components, const EntityManager>...> Entity::GetComponents() const {
			assert(Valid());
			return const_cast<const EntityManager*>(manager_)->GetComponents<const Components...>(id_);
		}


		template <typename C>
		bool Entity::HasComponent() const {
			assert(Valid());
			return manager_->HasComponent<C>(id_);
		}

		template <typename A, typename ... Args>
		void Entity::unpack(ComponentHandle<A> &a, ComponentHandle<Args> & ... args) {
			assert(Valid());
			manager_->unpack(id_, a, args ...);
		}

		inline bool Entity::Valid() const {
			return manager_ && manager_->Valid(id_);
		}

		inline std::ostream &operator << (std::ostream &out, const Entity::Id &id) {
			out << "Entity::Id(" << id.index() << "." << id.version() << ")";
			return out;
		}


		inline std::ostream &operator << (std::ostream &out, const Entity &entity) {
			out << "Entity(" << entity.id() << ")";
			return out;
		}


		template <typename C, typename EM>
		inline ComponentHandle<C, EM>::operator bool() const {
			return Valid();
		}

		template <typename C, typename EM>
		inline bool ComponentHandle<C, EM>::Valid() const {
			return manager_ && manager_->Valid(id_) && manager_->template HasComponent<C>(id_);
		}

		template <typename C, typename EM>
		inline C *ComponentHandle<C, EM>::operator -> () {
			assert(Valid());
			return manager_->template Get_component_ptr<C>(id_);
		}

		template <typename C, typename EM>
		inline const C *ComponentHandle<C, EM>::operator -> () const {
			assert(Valid());
			return manager_->template Get_component_ptr<C>(id_);
		}

		template <typename C, typename EM>
		inline C &ComponentHandle<C, EM>::operator * () {
			assert(Valid());
			return *manager_->template Get_component_ptr<C>(id_);
		}

		template <typename C, typename EM>
		inline const C &ComponentHandle<C, EM>::operator * () const {
			assert(Valid());
			return *manager_->template Get_component_ptr<C>(id_);
		}

		template <typename C, typename EM>
		inline C *ComponentHandle<C, EM>::Get() {
			assert(Valid());
			return manager_->template Get_component_ptr<C>(id_);
		}

		template <typename C, typename EM>
		inline const C *ComponentHandle<C, EM>::Get() const {
			assert(Valid());
			return manager_->template Get_component_ptr<C>(id_);
		}

		template <typename C, typename EM>
		inline void ComponentHandle<C, EM>::Remove() {
			assert(Valid());
			manager_->template Remove<C>(id_);
		}

		template <typename C, typename EM>
		inline Entity ComponentHandle<C, EM>::GetEntity() {
			assert(Valid());
			return manager_->Get(id_);
		}
	} 
}
namespace std {
template <> struct hash<NuclearEngine::ECS::Entity> {
  std::size_t operator () (const NuclearEngine::ECS::Entity &entity) const {
    return static_cast<std::size_t>(entity.id().index() ^ entity.id().version());
  }
};

template <> struct hash<const NuclearEngine::ECS::Entity> {
  std::size_t operator () (const NuclearEngine::ECS::Entity &entity) const {
    return static_cast<std::size_t>(entity.id().index() ^ entity.id().version());
  }
};
}

