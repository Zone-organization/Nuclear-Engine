#include <Engine\Rendering\RenderingPipeline.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine.h>
#include <Engine/Graphics/ImGui.h>
#include <Engine\Assets\Material.h>

namespace Nuclear
{
	namespace Rendering
	{
		RenderingPipeline::RenderingPipeline()
		{

		}
				
		void RenderingPipeline::InstantRender(Assets::Mesh* mesh, Assets::Material* material)
		{
			if (Engine::GetInstance()->isDebug())
			{
				if (mesh == nullptr)
				{
					NUCLEAR_ERROR("[RenderSystem DEBUG] Skipped Rendering invalid Mesh...");
					return;
				}
				if (material == nullptr)
				{
					NUCLEAR_ERROR("[RenderSystem DEBUG] Skipped Rendering Mesh with invalid Material...");
					return;
				}
				if (!material->GetShadingModel())
				{
					NUCLEAR_ERROR("[RenderSystem DEBUG] material->GetShadingModel() Skipped Rendering Mesh with invalid Material...");
					return;
				}
			}

			Uint64 offset = 0;

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{
				material->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

				DrawIndexedAttribs  DrawAttrs;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->DrawIndexed(DrawAttrs);

			}
		}

	}
}