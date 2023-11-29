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
	

	

	if (ImGui::Selectable("Koch Snowflake", m_Selected == 0, 0, ImVec2{ 120, 30 }))
	{
		m_Fractal = CreateRef<KochSnowfkale>();
		m_Selected = 0;
	}
	ImGui::SameLine();
	ImGui::ColorEdit3("##Snowflake color", glm::value_ptr(SnowflakeColor), ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine();
	ImGui::SliderInt("##Iterations1", &KochIterations, 1, 40);


	if (ImGui::Selectable("Mandelbrot", m_Selected == 1, 0, ImVec2{ 120, 30 }))
	{
		m_Fractal = CreateRef<MandelbrotSet>();
		m_Selected = 1;
	}
	ImGui::SameLine();
	ImGui::Dummy({ 25.0f, 0.0f });
	ImGui::SameLine();
	ImGui::SliderInt("##Iterations2", &MandelbrotIterations, 1, 100);



	if (ImGui::Selectable("Barnsley Fern", m_Selected == 2, 0, ImVec2{ 120, 30 }))
	{
		m_Fractal = CreateRef<BarnsleyFern>();
		m_Selected = 2;
	}
	ImGui::SameLine();
	ImGui::ColorEdit3("##Fern color", glm::value_ptr(FernColor), ImGuiColorEditFlags_NoInputs);

	ImGui::End();
}

