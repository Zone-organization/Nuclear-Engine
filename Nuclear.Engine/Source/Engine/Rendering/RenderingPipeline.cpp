#include <Engine\Rendering\RenderingPipeline.h>
#include <Engine\Graphics\Context.h>
#include <Core\Logger.h>
#include <Engine\Systems\CameraSubSystem.h>
#include <Engine\Assets\DefaultMeshes.h>
#include <Engine.h>
namespace Nuclear
{
	namespace Rendering
	{
		RenderingPipeline::RenderingPipeline(const std::string& name)
		{
			mName = name;
			mID = Utilities::Hash(mName);
		}

		void ParseForMatch(const std::unordered_map<Uint32, ShaderEffect>& first, const std::string& first_it_name, 
			const std::unordered_map<Uint32, ShaderEffect>& second, const std::string& second_it_name, std::unordered_map<Uint32, ShaderEffect>& result)
		{
			for (auto it1 : first)
			{
				//Check if id exists in result
				auto it = result.find(it1.first);
				if (it == result.end())                     //if not found
				{
					if (it1.second.GetType() == ShaderEffect::Type::PostProcessingAndRenderingEffect)
					{
						//Check if id exists in second
						auto it = second.find(it1.first);   
						if (it != second.end())               //if found
						{
							result[it1.first] = it1.second;   //Add new
						}
						else {                                //ID not found (mis match)
							Log.Warning("[RenderingPipeline] ShaderEffect: " + it1.second.GetName() + " In " + first_it_name + " has no match in " + second_it_name + "\n");
						}
					}
					else
					{
						auto it = result.find(it1.first);
						if (it != result.end())
						{
							assert(false);
						}
						//Add remaining effects
						result[it1.first] = it1.second;
					}
				}
			}
		}

		/*void RenderingPipeline::Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera, bool initshadow)
		{
			SetShadingModelAndCamera(shadingModel, camera);

			if (initshadow)
			{
				if (!m_pComparisonSampler)
				{
					SamplerDesc ComparsionSampler;
					ComparsionSampler.ComparisonFunc = COMPARISON_FUNC_LESS;
					// Note: anisotropic filtering requires SampleGrad to fix artifacts at
					// cascade boundaries
					ComparsionSampler.MinFilter = FILTER_TYPE_COMPARISON_LINEAR;
					ComparsionSampler.MagFilter = FILTER_TYPE_COMPARISON_LINEAR;
					ComparsionSampler.MipFilter = FILTER_TYPE_COMPARISON_LINEAR;
					Graphics::Context::GetDevice()->CreateSampler(ComparsionSampler, &m_pComparisonSampler);
				}
				ShadowMapManager::InitInfo SMMgrInitInfo;
				SMMgrInitInfo.Format = TEX_FORMAT_D16_UNORM;
				SMMgrInitInfo.Resolution = 1024;
				SMMgrInitInfo.NumCascades = 4;
				SMMgrInitInfo.ShadowMode = SHADOW_MODE_PCF;
				SMMgrInitInfo.pComparisonSampler = m_pComparisonSampler;
				m_ShadowMapMgr.Initialize(Graphics::Context::GetDevice(), SMMgrInitInfo);
			}

		}

		*/

		Graphics::RenderTarget* RenderingPipeline::GetSceneRT()
		{
			return &SceneRT;
		}

		IPipelineState* RenderingPipeline::GetActivePipeline()
		{
			return mActivePSO.RawPtr();
		}

		IShaderResourceBinding* RenderingPipeline::GetActiveSRB()
		{
			return mActiveSRB.RawPtr();
		}
		void RenderingPipeline::SetEffect(const Uint32& effectId, bool value)
		{
			//1. Find effect
			auto it = mPairedEffects.find(effectId);
			if (it != mPairedEffects.end())
			{
				it->second.SetValue(value);
				if (it->second.GetType() == ShaderEffect::Type::RenderingEffect)
				{
					mShadingModel->SetEffect(effectId, value);
				}
				else if (it->second.GetType() == ShaderEffect::Type::PostProcessingEffect)
				{
					SetPostProcessingEffect(effectId, value);
				}
				else if (it->second.GetType() == ShaderEffect::Type::PostProcessingAndRenderingEffect)
				{
					mShadingModel->SetEffect(effectId, value);
					SetPostProcessingEffect(effectId, value);
				}
			}
			else
			{
				//No match
			}
		}

		void RenderingPipeline::InstantRender(Assets::Mesh* mesh, Assets::Material* material)
		{
			if (Core::Engine::GetInstance()->isDebug())
			{
				if (mesh == nullptr)
				{
					Log.Error("[RenderSystem DEBUG] Skipped Rendering invalid Mesh...\n");
					return;
				}
				if (material == nullptr)
				{
					Log.Error("[RenderSystem DEBUG] Skipped Rendering Mesh with invalid Material...\n");
					return;
				}
			}

			Uint64 offset = 0;

			for (size_t i = 0; i < mesh->mSubMeshes.size(); i++)
			{
				material->GetMaterialInstance(GetShadingModel()->GetID())->BindTexSet(mesh->mSubMeshes.at(i).data.TexSetIndex);

				Graphics::Context::GetContext()->SetIndexBuffer(mesh->mSubMeshes.at(i).mIB, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
				Graphics::Context::GetContext()->SetVertexBuffers(0, 1, &mesh->mSubMeshes.at(i).mVB, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

				DrawIndexedAttribs  DrawAttrs;
				DrawAttrs.IndexType = VT_UINT32;
				DrawAttrs.NumIndices = mesh->mSubMeshes.at(i).mIndicesCount;
				Graphics::Context::GetContext()->DrawIndexed(DrawAttrs);

			}
		}

		void RenderingPipeline::SetShadingModelAndCamera(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera)
		{
			mShadingModel = shadingModel;
			mCamera = camera;

			//Step 1: Find Compatible Effects (build mPairedEffects map)
			ParseForMatch(mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mPostProcessingEffects, "PostProcessing", mPairedEffects);
			ParseForMatch(mPostProcessingEffects, "PostProcessing", mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mPairedEffects);
		}
		void RenderingPipeline::UpdatePSO(bool ForceDirty)
		{
			mRequiredHash = 0;

			if (mPipelineDirty || ForceDirty)
			{
				bool _hashzeroed = false;

				for (auto it : mPostProcessingEffects)
				{
					if (it.second.GetValue())
					{
						if (_hashzeroed)
						{
							mRequiredHash = 0;
							_hashzeroed = true;
						}
						auto val = mRequiredHash + it.second.GetID();

						mRequiredHash = val;
					}
				}

				auto PSO_SRB = mPipeline.GetVariant(mRequiredHash);

				mActivePSO = PSO_SRB.PSO;
				mActiveSRB = PSO_SRB.SRB;

				mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);
			}
		}

		Rendering::ShadingModel* RenderingPipeline::GetShadingModel() const
		{
			return mShadingModel;
		}
		Graphics::Camera* RenderingPipeline::GetCamera() const
		{
			return mCamera;
		}

		Uint32 RenderingPipeline::GetID() const
		{
			return mID;
		}
		std::string RenderingPipeline::GetName() const
		{
			return mName;
		}
		Graphics::BakeStatus RenderingPipeline::GetStatus() const
		{
			return mStatus;
		}
	}
}