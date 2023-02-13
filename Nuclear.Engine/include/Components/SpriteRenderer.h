#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <Graphics\ShaderPipelineSwitch.h>
#include <Serialization/Access.h>
#include <Serialization/SerializationEngine.h>
#include <Core\UUID.h>
#include <Assets/AssetType.h>
#include <Serialization/IsLoading.h>
#include <vector>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
		class Material;
	}

	namespace Components
	{

		class NEAPI SpriteRenderer
		{
		public:
			SpriteRenderer();
			SpriteRenderer(Assets::Texture* sprite, Assets::Material* material = nullptr);
			~SpriteRenderer();


		protected:
			friend Serialization::Access;

			Assets::Texture* pSprite;
			Assets::Material* pMaterial;
		};

	}
}