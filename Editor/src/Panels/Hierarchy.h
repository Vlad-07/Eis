#pragma once

#include <Eis/Core/Core.h>
#include <Eis/Scene/Scene.h>
#include <Eis/Scene/Entity.h>

namespace Eis
{
	class HierarchyPanel
	{
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Ref<Scene>& scene);


		void SetScene(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Scene;
		Entity m_Selection;
	};
}