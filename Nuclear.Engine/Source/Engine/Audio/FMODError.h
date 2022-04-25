#pragma once
#include <FMOD\inc\fmod_errors.h>
#include <Core\Logger.h>

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);

#define FMODCALL(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

#ifdef FMOD_ERRCHK_IMPL
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		//NUCLEAR_ERROR("[FMOD Error] File: '{0}' Line: '{1}' Error: '{3}'" , file , line , std::string(FMOD_ErrorString(result)));
	}
}
#endif