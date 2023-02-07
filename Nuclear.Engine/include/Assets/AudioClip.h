#pragma once
#include <Assets/IAsset.h>

namespace Nuclear
{
	namespace Assets
	{
		class NEAPI AudioClip : public IAsset
		{
		public:
			AudioClip();
			~AudioClip();

			FORCE_INLINE const Uint32 GetBufferID() const
			{
				return mBufferID;
			}

			FORCE_INLINE const bool Is3D() const
			{
				return m3D;
			}
	//	private:
			friend class Importer;

			bool m3D;
			bool mLoop;
			Uint32 mBufferID;
		};
	}
}