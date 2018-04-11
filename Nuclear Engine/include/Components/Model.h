#pragma once
#include <NE_Common.h>
#include <ECS\Entity.h>

namespace NuclearEngine {
	namespace XAsset {
		class ModelAsset;
	}
	namespace Components {

		class NEAPI Model : public ECS::Component<Model>
		{
		public:			
			Model();
			~Model();

			void SetAsset(XAsset::ModelAsset *asset);
			XAsset::ModelAsset * GetAsset();

		private:
			XAsset::ModelAsset * m_asset;
		};

	}
}