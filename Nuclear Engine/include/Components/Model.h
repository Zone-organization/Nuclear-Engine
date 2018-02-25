#pragma once
#include <NE_Common.h>
#include <Core\Entity.h>

namespace NuclearEngine {
	namespace Assets {
		class ModelAsset;
	}
	namespace Components {

		class NEAPI Model : public Core::Component<Model>
		{
		public:			
			Model();
			~Model();

			void SetAsset(Assets::ModelAsset *asset);
			Assets::ModelAsset * GetAsset();

		private:
			Assets::ModelAsset * m_asset;
		};

	}
}