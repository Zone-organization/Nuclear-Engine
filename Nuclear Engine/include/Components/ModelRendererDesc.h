#pragma once
#include <NE_Common.h>
#include <Components\Transform.h>
#include <Core\Entity.h>
#include <vector>
namespace NuclearEngine
{
	namespace Components
	{
		/*
		Describes how the render system should treat and render a '3D' model
		
		*/
		class NEAPI ModelRendererDesc : public Core::Component<ModelRendererDesc>
		{
		public:
			/*
			If size is 0, then it render the model with its basic transform 
			else
			it will render it multiple time with a new transform each time
			*/
			std::vector<Components::Transform> NumOfRenders = std::vector<Components::Transform>();
		};

	}
}

