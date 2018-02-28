#pragma once
#include <NE_Common.h>
#include <Core\Entity.h>

namespace NuclearEngine {
	namespace XAsset {
		class ModelAsset;
	}
	namespace Components {

		class NEAPI Model : public Core::Component<Model>
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