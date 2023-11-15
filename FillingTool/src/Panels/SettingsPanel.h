#pragma once
#include "Engine/Scene/Scene.h"
#include "Engine/Core/Core.h"
#include "Engine/Scene/Entity.h"

#include "SceneHierarchyPanel.h"

#include <glm/glm.hpp>
class SettingsPanel
{
public:
	SettingsPanel() = default;
	SettingsPanel(const Engine::Ref<Engine::Scene>& scene);

	void SetContext(const Engine::Ref<Engine::Scene>& scene);

	void SetSceneHierarchy(Engine::SceneHierarchyPanel* panel) { m_SceneHierarchy = panel; }

	void OnImGuiRender();

	glm::vec3 GetColor() { return m_DrawColor; }
	glm::vec3 GetBoundaryColor() {	return m_BoundaryColor;}
	void SetBoundaryColor(const glm::vec3& color) { m_BoundaryColor = color; }
	int32_t GetSelectedAlgorithm() { return m_SelectedAlgorithm; }
	void SetExecutionTime(float time) { m_AlgoTime = time; }

private:
	Engine::Ref<Engine::Scene> m_Context;
	Engine::SceneHierarchyPanel* m_SceneHierarchy;

	glm::vec3 m_DrawColor = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_BoundaryColor = { 0.0f, 0.0f, 0.0f };
	int32_t m_SelectedAlgorithm = -1;

	float m_AlgoTime = 0;

};