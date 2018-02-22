#pragma once
#include <NE_Common.h>
#include <Components\Model.h>
#include <Components\Transform.h>

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI GameObject
		{
		public:
			GameObject() {}
			~GameObject() {}

			void SetModel(Components::Model *m_model);
			Components::Model * GetModel();
			Transform* GetTransform();
		private:
			Components::Model *m_model;
			Transform m_transform;
		};

	}
}
