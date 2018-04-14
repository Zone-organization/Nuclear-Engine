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
		void Model::SetAsset(Assets::Mesh * asset)
		{
			m_asset = asset;
		}
		Assets::Mesh * Model::GetAsset()
		{
			return m_asset;
		}
	}
}