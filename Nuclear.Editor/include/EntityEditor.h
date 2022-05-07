#pragma once
#include <Common.h>

class NuclearEditor;

namespace Nuclear::Editor {
	class EntityEditor
	{
	public:
		EntityEditor();

		void Render(entt::entity& entity, entt::registry& reg, Components::EntityInfoComponent& Einfo);


	private:
		std::vector<entt::id_type> AllComponentsIDs;
	};
}