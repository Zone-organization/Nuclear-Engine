#pragma once
#include <unordered_map>
#include <Assets/Image.h>
#include <Serialization\Access.h>

//TODO: Probably not the best idea
#define MAX_MAP_SIZE UINT32_MAX

namespace Nuclear
{
	namespace Assets 
	{

#define AssetMapSerialization(T)											               \
	template<typename S>																	\
		void serialize(S& s, std::unordered_map<Uint32, T>& o)								\
		{																					\
			s.ext(o, Serialization::StdMap{ MAX_MAP_SIZE }, [](S& s, Uint32& key, T& value) \
				{ s.value4b(key); s.object(value); });										\
		}																					\



		AssetMapSerialization(Assets::Image)
	}
}