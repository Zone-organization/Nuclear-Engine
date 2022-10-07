#include "Engine\Components\MeshComponent.h"
#include <random>

std::random_device rd;     // Only used once to initialise (seed) engine
std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(0, 100); // Guaranteed unbiased

namespace Nuclear 
{
	namespace Components
	{
		MeshComponent::MeshComponent()
		{
			z = uni(rng);
			mMesh = nullptr;
			mMaterial = nullptr;
		}
		MeshComponent::MeshComponent(Assets::Mesh * mesh)
		{
			z = uni(rng);

			mMesh = mesh;
		}
		MeshComponent::MeshComponent(Assets::Mesh *mesh , Assets::Material * material)
		{
			z = uni(rng);

			mMesh = mesh;
			mMaterial = material;
		}
		MeshComponent::~MeshComponent()
		{
			mMesh = nullptr;
			mMaterial = nullptr;
		}
	}
}