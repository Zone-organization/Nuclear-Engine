#pragma once
#include <NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Assets/Shader.h>

//Engine effects
#define NE_BLOOM 0x4565a7fc

namespace Nuclear
{
	namespace Rendering
	{
		struct PostProcessingEffectBakingDesc
		{
			Uint32 mRTWidth = 800;
			Uint32 mRTHeight = 600;
			Graphics::RenderTargetDesc mFinalRTDesc;
		};

		enum class Type : Uint8
		{
			Unknown = 0,
			Bloom = 1
		};

		class NEAPI PostProcessingEffect
		{
		public:
			virtual void Bake(const PostProcessingEffectBakingDesc& desc) = 0;

			virtual void Update(FrameRenderData* framedata) = 0;

			virtual void ResizeRTs(Uint32 RTWidth, Uint32 RTHeight) {};

			virtual std::string GetName() = 0;

			virtual Uint32 GetHashedName() = 0;

			FORCE_INLINE void Enable(bool value)
			{
				mEnabled = value;
			}

			FORCE_INLINE bool isEnabled() const { return mEnabled; }
		protected:
			bool mEnabled = false;
		};
	}
}