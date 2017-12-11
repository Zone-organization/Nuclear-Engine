#pragma once
#include <NuclearCommon\Utilities\Logger.h>
#include <iostream>
#include <string>
#include <CompilerConfig.h>
#include <NE_Exception.h>

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace NuclearCommon;

namespace NuclearEngine {

	enum class AudioAPIs {
		OpenAL,
		XAudio,
		Auto,
		Unknown
	};

	static AudioAPIs *AudioAPI = new AudioAPIs();

	NEAPI void SetAudioAPI(AudioAPIs API);

	NEAPI AudioAPIs GetAudioAPI();
	NEAPI std::wstring String2WSTR(const std::string& s);


#define SDELETE(z)		if (z) {delete z; z = NULL;}
#define SADELETE(z)	    if (z) {delete[] z; z = NULL;}

}