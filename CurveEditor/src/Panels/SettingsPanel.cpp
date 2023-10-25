#include "SettingsPanel.h"

#include "Engine/Scene/Components.h"
#include <Engine/Core/Log.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "BezierCurveComponent.h"

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

	if (ImGui::Button("create point"))
	{
		Entity ent = m_SceneHierarchy->GetSelectedEntity();
		if (ent && ent.HasComponent<NativeScriptComponent>())
		{
			NativeScriptComponent& nsc = ent.GetComponent<NativeScriptComponent>();
			if (BezierCurveComponent* component = dynamic_cast<BezierCurveComponent*>(nsc.Instance); component)
			{
				component->CreatePoint();
			}
		}
	}

	ImGui::Text("HOW TO USE EDITOR:");
	ImGui::Text("");
	ImGui::Text("Click (RMB) Right Mouse Button on Scene Hierarchy -> Create Empty Entity");
	ImGui::Text("Select this Entity and click Add Component button in Properties Tab");
	ImGui::Text("Add Bezier Curve Component to create Bezier Curve");
	ImGui::Text("Add Sprite Renderer Component (optional) to change color");
	ImGui::Text("Select Draw Mode for Bezier Curve Entity in Properties tab");
	ImGui::Text("Click on Viewport to place control points");
	ImGui::Text("Select Cursor Mode and use LMB to drag control points");
	ImGui::Text("Click LMB on arrow before Entity node name to see control points node");
	ImGui::Text("Click RMB on control point node and select Delete Point to delete point");
	ImGui::Text("Use Control Point Properties Tab to move control points");
	ImGui::Text("Use Properties Tab to move, rotate, and scale Bezier Curve");
	ImGui::Text("Use negative value for Scale to inverse Curve");
	ImGui::Text("");
	ImGui::Text("Use WASD to navigate");
	ImGui::Text("Use MouseWheel to zoom");
	ImGui::End();
}

