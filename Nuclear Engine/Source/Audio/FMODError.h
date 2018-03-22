#pragma once
#include <FMOD\includer.h> 
#ifndef FMOD_NOT_INCLUDED

#include <FMOD\inc\fmod_errors.h>
#include <NE_Common.h>
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
#endif