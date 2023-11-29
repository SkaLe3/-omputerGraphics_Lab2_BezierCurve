#include "EditorLayer.h"
#include <imgui/imgui.h>
#include <Engine/Events/MouseEvent.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Utils/PlatformUtils.h"

#include "CameraController.h"
#include <Engine/Renderer/Shader.h>





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
	m_FramebufferTexture = Texture2D::Create(1600, 900);


	Application::Get().GetWindow().SetVSync(true);

	
	m_CameraEntity = m_ActiveScene->CreateEntity("Main Camera");
	m_CameraEntity.AddComponent<CameraComponent>();
	m_CameraEntity.GetComponent<CameraComponent>().Camera.SetOrthographic(m_ViewportSize.y, -5.0f, 5.0f);


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
		Renderer::OnWindowResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		m_FramebufferTexture = Texture2D::Create(m_ViewportSize.x, m_ViewportSize.y);
		uint8_t* textureData = m_Framebuffer->GetPixels();
		m_FramebufferTexture->SetData(textureData, m_ViewportSize.x * m_ViewportSize.y * 4);
		delete[] textureData;
	}
	// Update
	if (m_ViewportFocused)
	{

	}
	m_Controller.OnUpdate(ts);

	Engine::Renderer2D::ResetStats();


	m_Framebuffer->Bind();
	Engine::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Engine::RenderCommand::Clear();
	m_CameraEntity.GetComponent<CameraComponent>().Camera.SetOrthographic(m_ViewportSize.y, -5.0f, 5.0f);

	if (m_SettingsPanel.GetFractal())
	{
		if (Ref<KochSnowfkale> snowflake = dynamic_pointer_cast<KochSnowfkale>(m_SettingsPanel.GetFractal()); snowflake)
		{
			snowflake->Update(m_ViewportSize, m_Controller.translation, m_Controller.zoom, m_SettingsPanel.KochIterations, m_SettingsPanel.SnowflakeColor);
		}
		if (Ref<MandelbrotSet> mandelbrot = dynamic_pointer_cast<MandelbrotSet>(m_SettingsPanel.GetFractal()); mandelbrot)
		{
			mandelbrot->Update(m_ViewportSize, m_Controller.translation, m_Controller.zoom, m_SettingsPanel.MandelbrotIterations);
		}
		if (Ref<BarnsleyFern> fern = dynamic_pointer_cast<BarnsleyFern>(m_SettingsPanel.GetFractal()); fern)
		{
			fern->Update(m_ViewportSize, m_SettingsPanel.FernColor);

			if (Input::IsMouseButtonPressed(Mouse::Button0) && m_ViewportHovered)
				fern->GetShader()->SetInt("u_Reset", 1);
			else
				fern->GetShader()->SetInt("u_Reset", 0);
		}

		Renderer2D::BeginScene(m_CameraEntity.GetComponent<CameraComponent>().Camera, 
			m_CameraEntity.GetComponent<TransformComponent>().GetTransform(), m_SettingsPanel.GetFractal()->GetShader());
		Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, m_ViewportSize, m_FramebufferTexture);
		Renderer2D::EndScene();

		if (Ref<BarnsleyFern> fern = dynamic_pointer_cast<BarnsleyFern>(m_SettingsPanel.GetFractal()); fern)
		{
			m_FramebufferTexture = Texture2D::Create(m_ViewportSize.x, m_ViewportSize.y);
			uint8_t* textureData = m_Framebuffer->GetPixels();
			m_FramebufferTexture->SetData(textureData, m_ViewportSize.x * m_ViewportSize.y * 4);
			delete[] textureData;
		}

	}

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

	ImGui::Begin("Render Stats");
	ImGui::Text("FPS: %f", 1.0f / Application::Get().GetTimestep());
	ImGui::Text("");

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
	m_Controller.OnEvent(e);
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseClick));
	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseMoved));
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
	
	return false;
}

bool EditorLayer::OnMouseClick(MouseButtonPressedEvent& e)
{
	if ( m_ViewportHovered)
	{

	}
	return false;
}

bool EditorLayer::OnMouseMoved(MouseMovedEvent& e)
{
	if (!Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		return false;
	return false;

}


