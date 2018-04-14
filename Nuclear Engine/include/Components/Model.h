#pragma once
#include <NE_Common.h>
#include <ECS\Entity.h>

namespace NuclearEngine {
	namespace Assets {
		class ModelAsset;
	}
	namespace Components {

		class NEAPI Model : public ECS::Component<Model>
		{
		public:			
			Model();
			~Model();

			void SetAsset(Assets::Mesh *asset);
			Assets::Mesh * GetAsset();

		private:
			Assets::Mesh * m_asset;
		};

	}
}