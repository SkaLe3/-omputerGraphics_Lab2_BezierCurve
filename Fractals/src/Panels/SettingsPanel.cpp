#include "SettingsPanel.h"

#include "Engine/Scene/Components.h"
#include <Engine/Core/Log.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"


using namespace Engine;


void SettingsPanel::OnImGuiRender()
{
	ImGui::Begin("Settings");

	if (ImGui::Selectable("Koch Snowflake", m_Selected == 0, 0, ImVec2{ 350, 30 }))
		m_Fractal->CreateShader("assets/shaders/KochSnowflake.glsl");
	if (ImGui::Selectable("Mandelbrot", m_Selected == 0, 0, ImVec2{ 350, 30 }))
		m_Fractal->CreateShader("assets/shaders/Mandelbrot.glsl");

	ImGui::End();
}

