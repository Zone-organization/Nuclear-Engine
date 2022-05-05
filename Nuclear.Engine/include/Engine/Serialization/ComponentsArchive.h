#pragma once
#include <Engine/Serialization/MathArchive.h>
#include <Engine/Components/Components.h>
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>
#include <cereal\cereal.hpp>

namespace Nuclear
{
	namespace Components
	{
		template<class Archive> void serialize(Archive& archive, EntityInfoComponent& c)
		{ 
			archive(cereal::make_nvp("Name", c.mName));
			archive(cereal::make_nvp("Transform", c.mTransform));
		}


		template<class Archive> void serialize(Archive& archive, MeshComponent& c)
		{
			archive(cereal::make_nvp("Render", c.mRender));
		//	archive(cereal::make_nvp("Mesh ID", c.mMesh->mHashedName));
	//		archive(cereal::make_nvp("Mesh Name", c.mMesh->mName));

		//	archive(cereal::make_nvp("Material ID", c.mMaterial->mHashedName));
		//	archive(cereal::make_nvp("Material Name", c.mMaterial->mName));
		}
	}

	namespace ECS
	{
		template<class Archive> void serialize(Archive& archive, Transform& t)
		{
		//	archive(cereal::make_nvp("LocalPos", t.mLocalPosition));
		//	archive(cereal::make_nvp("LocalRot", t.mLocalRotation));
		//	archive(cereal::make_nvp("LocalScale", t.mLocalScale));
		//	archive(cereal::make_nvp("WorldPos", t.mWorldPosition));
		}
	}
}