#include <Engine\Rendering\RenderingPath.h>
#include <Engine\Rendering\FrameRenderData.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine.h>
#include <Engine/Graphics/ImGui.h>
#include <Engine\Assets\Material.h>
#include "Engine/Animation/Animator.h"
#include <Engine\Graphics\ShaderPipelineVariant.h>

namespace Nuclear
{
	namespace Rendering
	{
		RenderingPath::RenderingPath()
		{

		}

		void RenderingPath::BeginFrame(FrameRenderData* frame)
		{
			pCurrentFrame = frame;
			pActivePipeline = nullptr;
		}

		void RenderingPath::UpdateAnimationCB(Animation::Animator* animator)
		{
			if (pActivePipeline->isSkinned())
			{
				PVoid anim_data;
				Graphics::Context::GetContext()->MapBuffer(pCurrentFrame->pAnimationCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)anim_data);

				if (animator != nullptr)
				{
					std::vector<Math::Matrix4> ok;
					ok.reserve(100);

					auto transforms = animator->GetFinalBoneMatrices();
					for (int i = 0; i < transforms.size(); ++i)
					{
						ok.push_back(transforms[i]);
					}

					anim_data = memcpy(anim_data, ok.data(), ok.size() * sizeof(Math::Matrix4));
				}
				else
				{
					Math::Matrix4 empty(0.0f);
					anim_data = memcpy(anim_data, &empty, sizeof(Math::Matrix4));
				}
				Graphics::Context::GetContext()->UnmapBuffer(pCurrentFrame->pAnimationCB, MAP_WRITE);
			}
		}

		void RenderingPath::DrawMesh(Assets::Mesh* mesh, Assets::Material* material)
		{
			//Validation
			if (Engine::GetInstance()->isDebug())
			{
				if (mesh == nullptr)
				{
					NUCLEAR_ERROR("[RenderingPath] Skipped Rendering invalid Mesh...");
					return;
				}
				if (material->GetShaderID() != pActivePipeline->GetShaderAssetID())
				{
					NUCLEAR_ERROR("[RenderingPath] Skipped Rendering Mesh with invalid Material (shader asset & shader pipeline mismatch)...");
					return;
				}
			}
			Uint64 offset = 0;

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{
				material->BindTexSet(pActivePipeline, mesh->mSubMeshes.at(i).data.TexSetIndex);

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