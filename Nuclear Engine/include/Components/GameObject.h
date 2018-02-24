#pragma once
#include <NE_Common.h>
#include <Components\Model.h>
#include <Components\Transform.h>
#include <Core\Entity.h>
#include <Components\ModelRendererDesc.h>
namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI GameObject : public Core::Component<GameObject>
		{
		public:
			void SetModel(Components::Model *model);
			Components::Model *GetModel();
			void SetTransform(Transform transform);

			Transform* GetTransformComponent();

		private:
			Components::Model *m_model;
			Transform m_transform;
		};

	}
}
