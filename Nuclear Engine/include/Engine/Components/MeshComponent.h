#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>
#include <vector>

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
			MeshComponent();
			MeshComponent(Assets::Mesh *mesh , bool MultiRender = false);
			MeshComponent(Assets::Mesh *mesh , Assets::Material * material, bool MultiRender = false);
			~MeshComponent();

			//if false the RenderSystem won't render the model
			bool mRender = true;

			bool mMultiRender = false;
			std::vector<Math::Matrix4> mMultiRenderTransforms;

			Assets::Mesh * mMesh;
			Assets::Material * mMaterial;

		};

	}
}