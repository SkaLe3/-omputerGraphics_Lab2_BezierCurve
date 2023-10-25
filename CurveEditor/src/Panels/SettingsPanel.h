#pragma once
#include "Engine/Scene/Scene.h"
#include "Engine/Core/Core.h"
#include "Engine/Scene/Entity.h"

#include "SceneHierarchyPanel.h"
class SettingsPanel
{
public:
	SettingsPanel() = default;
	SettingsPanel(const Engine::Ref<Engine::Scene>& scene);

	void SetContext(const Engine::Ref<Engine::Scene>& scene);

	void SetSceneHierarchy(Engine::SceneHierarchyPanel* panel) { m_SceneHierarchy = panel; }

	void OnImGuiRender();

private:
	Engine::Ref<Engine::Scene> m_Context;
	Engine::SceneHierarchyPanel* m_SceneHierarchy;


};