#include <Engine\Assets\DefaultMeshes.h>
#include <Engine\Graphics\Context.h>

namespace Nuclear
{
	namespace Assets
	{
		static Assets::Mesh* gCube;
		static Assets::Mesh* gSphere;
		static Assets::Mesh* gPlane;
		static Assets::Mesh* gScreenQuad;

		void DefaultMeshes::Initialize()
		{
			gCube = new Assets::Mesh();
			gSphere = new Assets::Mesh();
			gPlane = new Assets::Mesh();
			gScreenQuad = new Assets::Mesh();
			Assets::Mesh::CreateCube(gCube);
			Assets::Mesh::CreateSphere(gSphere);
			Assets::Mesh::CreatePlane(gPlane, 50.f, 50.f);
			Assets::Mesh::CreateScreenQuad(gScreenQuad);

			gCube->SetName("Default Cube");
			gSphere->SetName("Default Sphere");
			gPlane->SetName("Default Plane");
			gScreenQuad->SetName("Default ScreenQuad");
		}
		Assets::Mesh* DefaultMeshes::GetCubeAsset()
		{
			return gCube;
		}
		Assets::Mesh* DefaultMeshes::GetSphereAsset()
		{
			return gSphere;
		}
		Assets::Mesh* DefaultMeshes::GetPlaneAsset()
		{
			return gPlane;
		}
		Assets::Mesh* DefaultMeshes::GetScreenQuadAsset()
		{
			return gScreenQuad;
		}
		void DefaultMeshes::RenderScreenQuad()
		{
			Uint64 offset = 0;
			Graphics::Context::GetContext()->SetIndexBuffer(gScreenQuad->mSubMeshes.at(0).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &gScreenQuad->mSubMeshes.at(0).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

			DrawIndexedAttribs DrawAttrs;
			DrawAttrs.IndexType = VT_UINT32;
			DrawAttrs.NumIndices = gScreenQuad->mSubMeshes.at(0).mIndicesCount;
			Graphics::Context::GetContext()->DrawIndexed(DrawAttrs);
		}
	}
}