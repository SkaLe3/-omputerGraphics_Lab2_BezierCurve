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
	ImGui::Text("Fill Color");
	ImGui::ColorEdit3("Draw color", glm::value_ptr(m_DrawColor));
	ImGui::Text("Boundary Color");
	ImGui::ColorEdit3("Boundary color", glm::value_ptr(m_BoundaryColor));



	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Selectable("Flood Fill", m_SelectedAlgorithm == 0, 0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 0;

	if (ImGui::Selectable("Boundary Fill", m_SelectedAlgorithm == 1,0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 1;

	if (ImGui::Selectable("Scanline Fill", m_SelectedAlgorithm == 2,0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 2;

	if (ImGui::Selectable("Span Fill", m_SelectedAlgorithm == 3,0, ImVec2(200, 25)))
		m_SelectedAlgorithm = 3;

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	if (m_AlgoTime != 0)
		ImGui::Text("Last algorithm execution time: %f (ms)", m_AlgoTime *1000);

	ImGui::End();
}

