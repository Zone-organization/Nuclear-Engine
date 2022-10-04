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

		//void ParseForMatch(const std::unordered_map<Uint32, ShaderEffect>& first, const std::string& first_it_name, 
		//	const std::unordered_map<Uint32, ShaderEffect>& second, const std::string& second_it_name, std::unordered_map<Uint32, ShaderEffect>& result)
		//{
		//	for (auto it1 : first)
		//	{
		//		//Check if id exists in result
		//		auto it = result.find(it1.first);
		//		if (it == result.end())                     //if not found
		//		{
		//			if (it1.second.GetType() == ShaderEffect::Type::PostProcessingAndRenderingEffect)
		//			{
		//				//Check if id exists in second
		//				auto it = second.find(it1.first);   
		//				if (it != second.end())               //if found
		//				{
		//					result[it1.first] = it1.second;   //Add new
		//				}
		//				else 
		//				{                                //ID not found (mis match)
		//					NUCLEAR_WARN("[RenderingPipeline] ShaderEffect: '{0}' in '{1}' has no match in '{2}'" , it1.second.GetName() , first_it_name ,  second_it_name);
		//				}
		//			}
		//			else
		//			{
		//				auto it = result.find(it1.first);
		//				if (it != result.end())
		//				{
		//					assert(false);
		//				}
		//				//Add remaining effects
		//				result[it1.first] = it1.second;
		//			}
		//		}
		//	}
		//}

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

				if (material->GetShadingModel()->GetID() != GetShadingModel()->GetID())
				{
					NUCLEAR_WARN("[RenderSystem DEBUG] material->GetShadingModel()->GetID() != GetShadingModel()->GetID()...");
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