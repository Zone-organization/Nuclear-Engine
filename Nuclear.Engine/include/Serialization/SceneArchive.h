#pragma once
#include <bitsery/bitsery.h>
#include <ECS/ECS.h>

namespace Nuclear
{
	namespace Serialization
	{

		//https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#archives
		template<class Serializer>
		class SceneOutputArchive
		{
		public:
			SceneOutputArchive(Serializer* const ser) : Ser(ser)
			{ }

			//Serialization
			void operator()(entt::entity e)
			{
				Ser->value4b(static_cast<Uint32>(e));
			}

			void operator()(std::underlying_type_t<entt::entity> e)
			{
				Ser->value4b(e);
			}

			template <class T> inline
			void operator()(entt::entity e, const T& c)
			{
				Ser->value4b(static_cast<Uint32>(e));
				Ser->object(c);
			}

			Serializer* const Ser;

		};

		template<class Serializer>
		class SceneInputArchive
		{
		public:
			SceneInputArchive(Serializer* const ser) : Ser(ser)
			{ }

			//De-Serialization
			void operator()(entt::entity& e)
			{
				entt::id_type id;
				Ser->value4b(id);
				e = static_cast<entt::entity>(id);
			}
			void operator()(std::underlying_type_t<entt::entity>& e)
			{
				Ser->value4b(e);
			}

			template <class T> inline
				void operator()(entt::entity& e, T& c)
			{
				entt::id_type id;
				Ser->value4b(id);
				e = static_cast<entt::entity>(id);
				Ser->object(c);
			}

			Serializer* const Ser;
		};
	}
}