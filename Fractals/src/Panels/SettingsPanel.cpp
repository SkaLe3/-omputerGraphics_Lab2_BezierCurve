#include "SettingsPanel.h"

#include "Engine/Scene/Components.h"
#include <Engine/Core/Log.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"


using namespace Engine;


SettingsPanel::SettingsPanel(EditorLayer* layer)
{
	m_Layer = layer;
}

void SettingsPanel::OnImGuiRender()
{
	ImGui::Begin("Settings");



	ImGui::End();
}

