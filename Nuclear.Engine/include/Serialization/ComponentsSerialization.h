#pragma once
#include <Serialization/MathSerialization.h>
#include <Components/Components.h>
#include <Assets/Mesh.h>
#include <Assets/Material.h>

namespace Nuclear
{
	namespace Components
	{
		template<class S> void serialize(S& s, EntityInfoComponent& c)
		{
			s.text1b(c.mName, 50);
			s.object(c.mTransform);
		}

		template<class Archive> void serialize(Archive& archive, MeshComponent& c)
		{
		//	archive(cereal::make_nvp("Render", c.mRender));
		//	archive(cereal::make_nvp("Mesh ID", c.mMesh->mHashedName));
	//		archive(cereal::make_nvp("Mesh Name", c.mMesh->mName));

		//	archive(cereal::make_nvp("Material ID", c.mMaterial->mHashedName));
		//	archive(cereal::make_nvp("Material Name", c.mMaterial->mName));
		}
	}

	//namespace ECS
	//{
	//	template<class S> void serialize(S& s, Transform& t)
	//	{
	//		s.object(t.mLocalPosition);
	//		s.object(t.mLocalRotation);
	//		s.object(t.mLocalScale);
	//		s.object(t.mWorldPosition);
	//	}
	//}
}