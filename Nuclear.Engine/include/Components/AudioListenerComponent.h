#pragma once
#include <NE_Common.h>
#include <Assets\AudioClip.h>

namespace Nuclear
{
	namespace Components
	{

		/*
		Multiple listeners in OpenAL is a problem, probable solutions:

		1-Multiple Contexts and change context each time a listener is activated 
		   downsides:
		   Expensive context changes
		   fixed-array allocated at initialization of contexts
		   recursive operations for each audio source function

		2-single context but multiple openal sources for each registered audio listener 
		    ex:
			 if max listeners are 8 then each audio source should have 8 sources
			downsides:
			 memory expensive?
			 can be more expensive than context switches if a scene has too much audio sources.

		*/
		class NEAPI AudioListenerComponent		//= OpenAL Context i guess
		{
		public:
			AudioListenerComponent();
			~AudioListenerComponent();


			//protected:
			Uint32 mContextID;
		};
	}
}