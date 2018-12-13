#pragma once
#include <Engine/Graphics/RenderObject.h>
#include <Engine/Graphics/Context.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		class NEAPI Renderer
		{
		public:


			//bool Create(RenderObject* object, )

			

			bool Validate(RenderObject* object);
			void Render(RenderObject* object);
		};
	}
}
