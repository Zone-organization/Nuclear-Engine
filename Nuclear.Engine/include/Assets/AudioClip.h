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

			const Uint32 GetBufferID() const;
		private:
			friend class Importer;

			Uint32 mBufferID;
		};
	}
}