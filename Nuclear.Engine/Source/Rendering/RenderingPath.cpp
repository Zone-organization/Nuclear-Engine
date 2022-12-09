#include <Rendering\RenderingPath.h>
#include <Rendering\FrameRenderData.h>
#include <Graphics\Context.h>
#include <Utilities/Logger.h>
#include <Assets\DefaultMeshes.h>
#include <Core/Engine.h>
#include <Graphics/ImGui.h>
#include <Assets\Material.h>
#include "Animation/Animator.h"
#include <Graphics\ShaderPipelineVariant.h>

namespace Nuclear
{
	namespace Rendering
	{
		RenderingPath::RenderingPath()
		{

		}

		void RenderingPath::UpdateAnimationCB(Animation::Animator* animator)
		{
			if (pActivePipeline->isSkinned())
			{
				PVoid anim_data;
				Graphics::Context::GetInstance().GetContext()->MapBuffer(pCurrentFrame->pAnimationCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)anim_data);

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
				Graphics::Context::GetInstance().GetContext()->UnmapBuffer(pCurrentFrame->pAnimationCB, MAP_WRITE);
			}
		}

		void RenderingPath::Reset()
		{
			pActivePipeline = nullptr;
			pCurrentFrame = nullptr;
		}

		void RenderingPath::DrawMesh(Assets::Mesh* mesh, Assets::Material* material)
		{
			//Validation
			if (Core::Engine::GetInstance().isDebug())
			{
				if (mesh == nullptr)
				{
					NUCLEAR_ERROR("[RenderingPath] Skipped Rendering invalid Mesh...");
					return;
				}
				if (material->GetShaderID() != pActivePipeline->GetShaderID())
				{
					NUCLEAR_ERROR("[RenderingPath] Skipped Rendering Mesh with invalid Material (shader asset & shader pipeline mismatch)...");
					return;
				}
			}
			Uint64 offset = 0;

			for (auto& i : mesh->mSubMeshes)
			{
				material->BindTexSet(pActivePipeline, i.data.TexSetIndex);

				Graphics::Context::GetInstance().GetContext()->SetIndexBuffer(i.mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetInstance().GetContext()->SetVertexBuffers(0, 1, &i.mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

				DrawIndexedAttribs  DrawAttrs;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = i.mIndicesCount;
				Graphics::Context::GetInstance().GetContext()->DrawIndexed(DrawAttrs);

			}
		}
	}
}