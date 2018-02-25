#include "Components\Model.h"

namespace NuclearEngine 
{
	namespace Components
	{
		Model::Model()
		{
			m_asset = nullptr;
		}
		Model::~Model()
		{
			m_asset = nullptr;
		}
		void Model::SetAsset(Assets::ModelAsset * asset)
		{
			m_asset = asset;
		}
		Assets::ModelAsset * Model::GetAsset()
		{
			return m_asset;
		}
	}
}