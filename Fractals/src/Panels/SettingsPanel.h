#pragma once
#include <Engine.h>

#include <glm/glm.hpp>
#include "Fractals.h"

class EditorLayer;

class SettingsPanel
{
public:
	SettingsPanel() = default;

	void OnImGuiRender();
	void SetContext(Engine::Ref<Fractal> fr) { m_Fractal = fr; }

private:
	Engine::Ref<Fractal> m_Fractal;
	int32_t m_Selected;
};