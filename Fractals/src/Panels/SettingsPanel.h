#pragma once
#include <Engine.h>

#include <glm/glm.hpp>
#include "Fractals/Fractals.h"

class EditorLayer;

class SettingsPanel
{
public:
	SettingsPanel() = default;

	void OnImGuiRender();
	void SetContext(Engine::Ref<Fractal> fr) { m_Fractal = fr; }
	Engine::Ref<Fractal> GetFractal() { return m_Fractal; }

private:
	Engine::Ref<Fractal> m_Fractal;
	int32_t m_Selected;
public:
	int32_t KochIterations = 5;
	int32_t MandelbrotIterations = 20;

	glm::vec3 FernColor = { 1.0f, 1.0f, 1.0f };
	glm::vec3 SnowflakeColor = { 1.0f, 1.0f, 1.0f };
};