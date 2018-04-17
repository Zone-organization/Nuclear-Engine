#pragma once
#include <NE_Common.h>
#include <ECS\Entity.h>

namespace NuclearEngine {
	namespace Assets 
	{
		class Mesh;
		class Material;
	}
	namespace Components 
	{

		class NEAPI MeshComponent : public ECS::Component<MeshComponent>
		{
		public:			
			MeshComponent(Assets::Mesh *mesh = nullptr, Assets::Material * material = nullptr);
			~MeshComponent();

			//if false the RenderSystem won't render the model
			bool Render = true;

			Assets::Mesh * mMesh;
			Assets::Material * mMaterial;

		};

	}
}