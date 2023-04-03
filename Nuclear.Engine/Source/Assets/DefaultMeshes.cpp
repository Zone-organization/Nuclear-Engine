#include <Assets\DefaultMeshes.h>
#include <Graphics/GraphicsModule.h>

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
			Assets::Mesh::CreatePlane(gPlane, 25.f, 25.f);
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
			Graphics::GraphicsModule::Get().GetContext()->SetIndexBuffer(gScreenQuad->mSubMeshes.at(0).mIB, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			Graphics::GraphicsModule::Get().GetContext()->SetVertexBuffers(0, 1, &gScreenQuad->mSubMeshes.at(0).mVB, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);

			Diligent::DrawIndexedAttribs DrawAttrs;
			DrawAttrs.IndexType = Diligent::VT_UINT32;
			DrawAttrs.NumIndices = gScreenQuad->mSubMeshes.at(0).mIndicesCount;
			Graphics::GraphicsModule::Get().GetContext()->DrawIndexed(DrawAttrs);
		}
	}
}