#pragma once

#include "Engine.h"
#include "Engine/Renderer/EditorCamera.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SettingsPanel.h"
// temp
#include <imgui/imgui.h>
using namespace Engine;

class EditorLayer : public Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;
private:
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnMouseClick(MouseButtonPressedEvent& e);
	bool OnMouseMoved(MouseMovedEvent& e);

	glm::vec2 WindowToWorld(const glm::vec2& windowCoords);
private:

	Engine::Ref<Engine::Scene> m_ActiveScene;
	Entity m_CameraEntity;

	Ref<Framebuffer> m_Framebuffer;

	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportPos;
	// Image
	Entity m_ImageEntity;
	//
	ImVec2 m_WindowPos;

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	SettingsPanel m_SettingsPanel;
};