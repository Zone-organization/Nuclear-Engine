#include "Components\MeshComponent.h"

namespace NuclearEngine 
{
	namespace Components
	{
		MeshComponent::MeshComponent(Assets::Mesh * asset)
		{
			m_asset = asset;
		}
		MeshComponent::MeshComponent()
		{
			m_asset = nullptr;
		}
		MeshComponent::~MeshComponent()
		{
			m_asset = nullptr;
		}
		void MeshComponent::SetAsset(Assets::Mesh * asset)
		{
			m_asset = asset;
		}
		Assets::Mesh * MeshComponent::GetAsset()
		{
			return m_asset;
		}
	}
}