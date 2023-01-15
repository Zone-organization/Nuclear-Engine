#pragma once
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
			{
			}

			//Serialization
			ZPP_BITS_INLINE constexpr auto operator()(entt::entity e)
			{
				return Ser->serialize_one(static_cast<Uint32>(e));
			}

			ZPP_BITS_INLINE constexpr auto operator()(std::underlying_type_t<entt::entity> size)
			{
				return Ser->serialize_one(size);
			}

			template <class T> inline
				constexpr auto operator()(entt::entity e, const T& c)
			{
				Ser->serialize_one(static_cast<Uint32>(e));
				return Ser->serialize_one(c);
			}

			Serializer* const Ser;

		};

		template<class Serializer>
		class SceneInputArchive
		{
		public:
			SceneInputArchive(Serializer* const ser) : Ser(ser)
			{
			}

			//De-Serialization
			ZPP_BITS_INLINE constexpr auto operator()(entt::entity& e)
			{
				entt::id_type id;
				auto result = Ser->serialize_one(id);
				e = static_cast<entt::entity>(id);
				return result;
			}
			ZPP_BITS_INLINE constexpr auto operator()(std::underlying_type_t<entt::entity>& s)
			{
				//	entt::id_type size;
				return Ser->serialize_one(s);

				//	s = static_cast<std::underlying_type_t<entt::entity>>(size);
			}

			template <class T> inline
				constexpr auto operator()(entt::entity& e, T& c)
			{
				entt::id_type id;
				Ser->serialize_one(id);
				e = static_cast<entt::entity>(id);
				return Ser->serialize_one(c);
			}

			Serializer* const Ser;
		};
	}
}