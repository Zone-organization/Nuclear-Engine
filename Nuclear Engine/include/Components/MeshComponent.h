#pragma once
#include <NE_Common.h>
#include <ECS\Entity.h>

namespace NuclearEngine {
	namespace Assets {
		class Mesh;
	}
	namespace Components {

		class NEAPI MeshComponent : public ECS::Component<MeshComponent>
		{
		public:			
			MeshComponent(Assets::Mesh *asset);
			MeshComponent();
			~MeshComponent();

			void SetAsset(Assets::Mesh *asset);
			Assets::Mesh * GetAsset();

		private:
			Assets::Mesh * m_asset;
		};

	}
}