#include <Engine\Rendering\RenderingPipeline.h>
#include <Core\Logger.h>

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
					if (it1.second.GetType() == ShaderEffect::Type::CameraAndRenderingEffect)
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


				//if (it1.second.GetType() == ShaderEffect::Type::CameraAndRenderingEffect)
				//{
				//	bool matchfound = false;
				//	for (auto it2 : second)
				//	{
				//		if (!matchfound)
				//		{
				//			//Check if exists
				//			auto it = result.find(it1.second.GetID());
				//			if (it == result.end())
				//			{
				//				if (it1.second.GetID() == it2.second.GetID())
				//				{
				//					result[it1.second.GetID()] = it1.second;
				//					matchfound = true;
				//				}
				//			}
				//		}
				//	}

				//	if (!matchfound)
				//	{
				//		//Log.Warning("[RenderingPipeline] Shading Model: " + std::to_string(shadingModel->GetID()) +
				//		//	" ShaderEffect: " + sm_it.GetName() + " Has no match\n");
				//		Log.Warning("[RenderingPipeline] ShaderEffect: " + it1.second.GetName() + " In " + first_it_name + " has no match in " + second_it_name + "\n");
				//	}
				//}
				//else 
				//{
				//	auto it = result.find(it1.second.GetID());
				//	if (it != result.end())
				//	{
				//		assert(false);
				//	}
				//	//Add remaining effects
				//	result[it1.second.GetID()] = it1.second;
				//}
			}
		}

		void RenderingPipeline::Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera)
		{
			mShadingModel = shadingModel;
			mCamera = camera;


			//Step 1: Find Compatible Effects (build mPairedEffects map)
			ParseForMatch(mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mCamera->mCameraEffects, "Camera", mPairedEffects);
			ParseForMatch(mCamera->mCameraEffects,"Camera", mShadingModel->GetRenderingEffects(), mShadingModel->GetName(), mPairedEffects);

		}
		void RenderingPipeline::SetEffect(const Uint32& effectId, bool value)
		{
			//1. Find effect
			auto it = mPairedEffects.find(effectId);
			if (it != mPairedEffects.end())
			{
				if (it->second.GetType() == ShaderEffect::Type::RenderingEffect)
				{
					mShadingModel->SetEffect(effectId, value);
				}
				else if (it->second.GetType() == ShaderEffect::Type::CameraEffect)
				{
					mCamera->SetEffect(effectId, value);
				}
				else if (it->second.GetType() == ShaderEffect::Type::CameraAndRenderingEffect)
				{
					mShadingModel->SetEffect(effectId, value);
					mCamera->SetEffect(effectId, value);
				}
			}
			else
			{
				//No match
			}
		}
		Rendering::ShadingModel* RenderingPipeline::GetShadingModel()
		{
			return mShadingModel;
		}
		Graphics::Camera* RenderingPipeline::GetCamera()
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
	}
}