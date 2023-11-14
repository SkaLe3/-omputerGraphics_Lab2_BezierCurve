#include "SettingsPanel.h"

#include "Engine/Scene/Components.h"
#include <Engine/Core/Log.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>


using namespace Engine;

SettingsPanel::SettingsPanel(const Engine::Ref<Engine::Scene>& scene)
{
	SetContext(scene);
}

void SettingsPanel::SetContext(const Engine::Ref<Engine::Scene>& scene)
{
	m_Context = scene;
}

void SettingsPanel::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorPicker3("Draw color", m_DrawColor);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Selectable("FloodFill", m_SelectedAlgorithm == 0, 0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 0;

	if (ImGui::Selectable("2", m_SelectedAlgorithm == 1,0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 1;

	if (ImGui::Selectable("3", m_SelectedAlgorithm == 2,0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 2;

	if (ImGui::Selectable("4", m_SelectedAlgorithm == 3,0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 3;



	ImGui::End();
}

