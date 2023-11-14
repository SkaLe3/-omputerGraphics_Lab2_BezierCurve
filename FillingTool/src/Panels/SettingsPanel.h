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

	glm::vec3 GetColor() { return glm::vec3(m_DrawColor[0], m_DrawColor[1], m_DrawColor[2]); }
	int32_t GetSelectedAlgorithm() { return m_SelectedAlgorithm; }
	

private:
	Engine::Ref<Engine::Scene> m_Context;
	Engine::SceneHierarchyPanel* m_SceneHierarchy;

	float m_DrawColor[3] = { 0.0f, 0.0f, 0.0f };
	int32_t m_SelectedAlgorithm = -1;


};