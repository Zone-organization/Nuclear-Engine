#pragma once
#include <Audio/AudioBackend.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <Compiler.h>

namespace Nuclear
{
	namespace Audio
	{

		struct XAudioSource
		{
			IXAudio2SourceVoice* pVoice = nullptr;
			X3DAUDIO_EMITTER m3D_Data;
			WAVEFORMATEX mFormat;
			bool mDirty = false;

			FORCE_INLINE bool Unused() const
			{
				return (pVoice == nullptr);
			}
		};

		struct XAudioBuffer
		{
			AudioFile mFile;
			bool Used = false;
		};

		class NEAPI XAudioBackend : public AudioBackend
		{
		public:
			XAudioBackend();
			~XAudioBackend();

			bool Initialize() override;

			void Shutdown() override;
			bool CreateAudioClip(Uint32& result, AudioFile& file) override;

			bool CreateAudioSource(Uint32& source) override;

			void SetAudioSourceClip(const Uint32 source, const Uint32 clip) override;

			void Play(const Uint32 source) override;

			void Pause(const Uint32 source) override;

			void Stop(const Uint32 source) override;

			void SetSource_Volume(const Uint32 audio_source, float vol) override;

			void SetSource_Pitch(const Uint32 audio_source, float pitch) override;

			void SetSource_IsLooping(const Uint32 audio_source, bool val) override;

			void SetSource_Transform(const Uint32 audio_source, const Math::Vector3& pos, const Math::Quaternion& rot) override;

			void SetSource_Velocity(const Uint32 audio_source, const Math::Vector3& val) override;

			void SetListener_Velocity(const Math::Vector3& val) override;

			void SetListener_Transform(const Math::Vector3& pos, const Math::Quaternion& rot) override;

			void Update();

		protected:
			XAudioSource& GetSource(const Uint32 id);
			XAudioBuffer& GetBuffer(const Uint32 id);

			XAudioSource* GetSourcePtr(const Uint32 audio_source);
			//XAudioBuffer* GetBufferFromSource(const Uint32 audio_source);
			//XAudioBuffer* GetBufferPtr(const Uint32 clip);

			IXAudio2* pXAudio2;
			IXAudio2MasteringVoice* pMasterVoice;
			X3DAUDIO_HANDLE X3DInstance;
			X3DAUDIO_LISTENER m3DListener;

			std::vector<XAudioSource> mSources;
			std::vector<XAudioBuffer> mBuffers;
		};
	}
}