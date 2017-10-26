#include "NE_Common.h"
#include <Windows.h>
namespace NuclearEngine {
	NEAPI void SetAudioAPI(AudioAPIs API)
	{
		*AudioAPI = API;
		return;
	}
	
	NEAPI AudioAPIs GetAudioAPI()
	{
		if (*AudioAPI == AudioAPIs::OpenAL)
		{
			return AudioAPIs::OpenAL;
		}
		else if (*AudioAPI == AudioAPIs::XAudio)
		{
			return  AudioAPIs::XAudio;
		}
		else if (*AudioAPI == AudioAPIs::Auto)
		{
			return  AudioAPIs::Auto;
		}
		return AudioAPIs::Unknown;
	}

	NEAPI std::wstring String2WSTR(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

}