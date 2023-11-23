#pragma once

#include "Engine.h"
#include "Engine/Renderer/EditorCamera.h"

#include "Panels/SettingsPanel.h"
#include <glm/glm.hpp>
#include "CameraController.h"
#include "Fractals.h"

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
private:

	Engine::Ref<Engine::Scene> m_ActiveScene;
	Entity m_CameraEntity;

	CameraController m_Controller;

	Ref<Framebuffer> m_Framebuffer;

	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	// Shaders
	Ref<Fractal> m_Fractal;


	// Panels
	SettingsPanel m_SettingsPanel;
};