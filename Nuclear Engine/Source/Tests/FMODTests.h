#pragma once
#include <Graphics/API/DirectX\DX11Context.h>
#include "TestCommon.h"
#include <FMOD\includer.h> 

#ifndef _DEBUG 
class FMODTests : public Core::Game
{
};
#elif defined(FMOD_NOT_INCLUDED)
class FMODTests : public Core::Game
{
};
#else
#include <iostream>
#include <FMOD\inc\fmod.hpp>
#include <FMOD\inc\fmod_errors.h>
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		Log.Error("[AudioSystem] FMOD Error Info: " + std::string(FMOD_ErrorString(result)) + "\n");
	}
}

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class FMODTests : public Core::Game
{
protected:
	FMOD::System     *system;
	FMOD::Sound      *sound1;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;

public:
	FMODTests()
	{
	}
	void Load()
	{
		/*
		Create a System object and initialize
		*/
		result = FMOD::System_Create(&system);
		ERRCHECK(result);

		result = system->init(32, FMOD_INIT_NORMAL,0);
		ERRCHECK(result);

		result = system->createSound("Assets/Common/Sounds/yurimaster.wav", FMOD_DEFAULT, 0, &sound1);
		ERRCHECK(result);

		result = sound1->setMode(FMOD_LOOP_OFF);    /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
		ERRCHECK(result);                           /* so turn it off here.  We could have also just put FMOD_LOOP_OFF in the above CreateSound call. */

	}

	void Render(float) override
	{
		//if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num1))
		//{
			result = system->playSound(sound1, 0, false, &channel);
			ERRCHECK(result);
		//}

		result = system->update();
		ERRCHECK(result);

		//{
		//	unsigned int lenms = 0;
		//	bool         playing = 0;
		//	bool         paused = 0;

		//	if (channel)
		//	{

		//		result = channel->isPlaying(&playing);
		//		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		//		{
		//			ERRCHECK(result);
		//		}

		//		result = channel->getPaused(&paused);
		//		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		//		{
		//			ERRCHECK(result);
		//		}
		//					
		//	}
		//}

	}
};

#endif
