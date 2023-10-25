#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Utils/PlatformUtils.h"
#include "Algorithms/Drawer.h"

#include "CameraController.h"
#include "BezierCurveComponent.h"
#include <ctime>



EditorLayer::EditorLayer()
	: Layer()
{
}

void EditorLayer::OnAttach()
{
	FramebufferSpecification fbSpec;
	fbSpec.Width = 1600;
	fbSpec.Height = 900;
	m_Framebuffer = Framebuffer::Create(fbSpec);
	m_ActiveScene = CreateRef<Scene>();


	Application::Get().GetWindow().SetVSync(false);

	
	m_CameraEntity = m_ActiveScene->CreateEntity("Main Camera");
	m_CameraEntity.AddComponent<CameraComponent>();
	m_CameraEntity.GetComponent<CameraComponent>().Camera.SetOrthographic(300.0f, -5.0f, 5.0f);
	m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();



	m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	m_SettingsPanel.SetContext(m_ActiveScene);
	m_SettingsPanel.SetSceneHierarchy(&m_SceneHierarchyPanel);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(Timestep ts)
{
	// Resize
	if (Engine::FramebufferSpecification spec = m_Framebuffer->GetSpecificaion();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

	}
	// Update
	if (m_ViewportFocused)
	{

	}

	Engine::Renderer2D::ResetStats();
	m_Framebuffer->Bind();
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RenderCommand::Clear();

	m_ActiveScene->OnUpdateRuntime(ts);
	m_Framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{

	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
// all active windows docked into it will lose their parent and become undocked.
// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMinSize.x = 350.0f;

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = 32.0f;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	m_SceneHierarchyPanel.OnImGuiRender();

	ImGui::Begin("Render Stats");
	ImGui::Text("FPS: %f", 1.0f / Application::Get().GetTimestep());
	ImGui::Text("");
	ImGui::Text("Curve Stats:");
	//ImGui::Text("Curve Render Time (ms): %.2f", duration * 1000);
	//ImGui::Text("Control points count: %d" ,points.size());
	auto stats = Engine::Renderer2D::GetStats();
	ImGui::Text("");
	ImGui::Text("Renderer Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	


	ImVec2 poss = ImGui::GetWindowPos();

	int32_t glfwposx  = Application::Get().GetWindow().GetWindowPositionX();
	int32_t glfwposy = Application::Get().GetWindow().GetWindowPositionY();

	m_ViewportPos = { poss.x - glfwposx, poss.y - glfwposy };


	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);


	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };


	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();

	m_SettingsPanel.OnImGuiRender();
}

void EditorLayer::OnEvent(Engine::Event& e)
{
	static_cast<CameraController*>(m_CameraEntity.GetComponent<NativeScriptComponent>().Instance)->OnEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseClick));
	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseMoved));
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
	
	return true;
}

bool EditorLayer::OnMouseClick(MouseButtonPressedEvent& e)
{
	if ( m_ViewportHovered)
	{
		m_SceneHierarchyPanel.OnViewportClick(WindowToWorld(Input::GetMousePosition()));	
	}
	return true;
}

bool EditorLayer::OnMouseMoved(MouseMovedEvent& e)
{
	if (!Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		return false;

	Entity point = m_SceneHierarchyPanel.GetSelectedControlPoint();
	if (point)
	{
		Entity owner = m_SceneHierarchyPanel.GetSelectedControlPointOwner();
		if (e.GetX() != 0 || e.GetY() != 0)
		{
			m_SceneHierarchyPanel.DragPoint(point, owner, WindowToWorld({ e.GetX(), e.GetY() }));
		}
	}
	return true;

}

glm::vec2 EditorLayer::WindowToWorld(const glm::vec2& windowCoords)
{
	glm::vec2 coords;
	coords.x = windowCoords.x - m_ViewportPos.x;
	coords.y = windowCoords.y - m_ViewportPos.y;
	
	glm::vec2 openglCoords;
	openglCoords.x = coords.x / m_ViewportSize.x * 2.0f - 1.0f;
	openglCoords.y = (m_ViewportSize.y - coords.y) / m_ViewportSize.y * 2.0f - 1.0f;

	glm::mat4 transform = glm::inverse(m_CameraEntity.GetComponent<CameraComponent>().Camera.GetProjection() * glm::inverse(m_CameraEntity.GetComponent<TransformComponent>().GetTransform()));
	glm::vec4 worldCoords = transform * glm::vec4{ openglCoords.x, openglCoords.y, 0, 1 };

	return glm::vec2{ worldCoords.x, worldCoords.y };
}

