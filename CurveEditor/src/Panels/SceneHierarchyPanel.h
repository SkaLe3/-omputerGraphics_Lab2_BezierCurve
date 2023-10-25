#pragma once
#include "Engine/Scene/Scene.h"
#include "Engine/Core/Core.h"
#include "Engine/Scene/Entity.h"

namespace Engine {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		Entity GetSelectedControlPoint() const { return m_SelectedControlPoint; }
		Entity GetSelectedControlPointOwner() const { return m_SelectedControlPointOwner; }

		void DragPoint(Entity point, Entity owner, const glm::vec2& position);

		void OnViewportClick(const glm::vec2& coords);
	private:
		void DrawEntityNode(Entity entity);
		void DrawControlPointComponents(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;

		Entity m_SelectionContext;
		Entity m_SelectedControlPoint;
		Entity m_SelectedControlPointOwner;

	};
}