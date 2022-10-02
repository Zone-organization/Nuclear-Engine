#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine/Graphics/RenderTarget.h>
#include <vector>

namespace Nuclear
{
    namespace Assets {
        class Mesh;
        class Material;
    }

	namespace Rendering
	{

        struct DrawCommand
        {
            DrawCommand(Assets::Mesh* mesh, Assets::Material* material, const  Math::Matrix4& transform)
                : pMesh(mesh), pMaterial(material), mWorldTransform(transform) {}

            Assets::Mesh* GetMesh() const { return pMesh; }
            Assets::Material* GetMaterial() const { return pMaterial; }
            const Math::Matrix4& GetTransform() const { return mWorldTransform; }

        private:
            Assets::Mesh* pMesh;
            Assets::Material* pMaterial;
            Math::Matrix4 mWorldTransform;
        };

		struct NEAPI FrameRenderData
		{
		public:
			


            //Rendered Scene here
            Graphics::RenderTarget mFinalRT;
            Graphics::RenderTarget mFinalDepthRT;
		};
	}
}