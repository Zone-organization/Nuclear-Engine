#pragma once
#include <FMOD\inc\fmod_errors.h>
#include <Base\NE_Common.h>
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);

#define FMODCALL(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

#ifdef FMOD_ERRCHK_IMPL
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		NuclearEngine::Log.Error("[FMOD Error] File: " + std::string(file) + " Line: " + std::to_string(line) + " Error: " + std::string(FMOD_ErrorString(result)) + "\n");
	}
}
#endif