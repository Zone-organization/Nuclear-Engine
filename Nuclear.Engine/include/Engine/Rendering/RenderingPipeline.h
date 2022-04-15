#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Camera.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Engine/Rendering/ShadingModel.h>
#include <unordered_map>

namespace Nuclear
{
	namespace Rendering
	{

		//A class of compatible camera and shading model
		class NEAPI RenderingPipeline
		{
		public:
			RenderingPipeline(const std::string& name);

			void Initialize(Rendering::ShadingModel* shadingModel, Graphics::Camera* camera);

			//std::vector<ShaderEffect> mInCompatibleEffects;
			std::unordered_map<Uint32, ShaderEffect> mPairedEffects;

			//void SetEffect(const std::string& effectname, bool value);
			void SetEffect(const Uint32& effectId, bool value);

			Rendering::ShadingModel* GetShadingModel();
			Graphics::Camera* GetCamera();

			void Bake(ShadingModelBakingDesc& desc);

			Uint32 GetID() const;
			std::string GetName() const;
		protected:
			Rendering::ShadingModel* mShadingModel;
			Graphics::Camera* mCamera;
			Uint32 mID;
			std::string mName;
		};
	}
}