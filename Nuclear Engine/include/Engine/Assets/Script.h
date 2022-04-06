#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets/Common.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI Script : public Asset<Script>
		{
		public:
			Script();
			~Script();

			void* internalScriptPtr;

			bool HasStartFunc = false;
			bool HasUpdateFunc = false;

		};
	}
}

/*

	void Start() //script initializaton
	{

	}

	void Update() //once per frame
	{

	}
*/