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

	ImGui::End();
}

