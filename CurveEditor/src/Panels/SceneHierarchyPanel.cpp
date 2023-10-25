#include "Panels/SceneHierarchyPanel.h"

#include "Engine/Scene/Components.h"
#include <Engine/Core/Log.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "BezierCurveComponent.h"

namespace Engine {


	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				if (!entity.HasComponent<ControlPointComponent>())
					DrawEntityNode(entity);

			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}



		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();

		ImGui::Begin("Control Point Properties");
		if (m_SelectedControlPoint)
		{
			DrawControlPointComponents(m_SelectedControlPoint);
		}
		ImGui::End();
	}


	void SceneHierarchyPanel::DragPoint(Entity point, Entity owner, const glm::vec2& position)
	{
		point.GetComponent<TransformComponent>().Translation = { position.x, position.y, 0.0f };

		static_cast<BezierCurveComponent*>(owner.GetComponent<NativeScriptComponent>().Instance)->UpdateRelativeCoordinates();
	}

	void SceneHierarchyPanel::OnViewportClick(const glm::vec2& coords)
	{
		if (m_SelectionContext && m_SelectionContext.HasComponent<NativeScriptComponent>())
		{

			BezierCurveComponent* bcc = dynamic_cast<BezierCurveComponent*>(m_SelectionContext.GetComponent<NativeScriptComponent>().Instance);
			if (bcc)
			{
				if (bcc->DrawEnabled)
				{
					bcc->CreatePoint({ coords.x, coords.y, 0 });
					return;
				}
			}
		}

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
		if (entity.HasComponent<NativeScriptComponent>())
		{
			BezierCurveComponent* comp = dynamic_cast<BezierCurveComponent*>(entity.GetComponent<NativeScriptComponent>().Instance);
			if (comp)
			{
				bool hit = false;
				std::list<Entity>& points = comp->GetPoints();
				for (Entity point : points)
				{
					glm::vec3 pos = point.GetComponent<TransformComponent>().Translation;
					glm::vec3 scale = point.GetComponent<TransformComponent>().Scale;
					if (coords.x < pos.x + scale.x && coords.x > pos.x - scale.x &&
						coords.y < pos.y + scale.y && coords.y > pos.y - scale.y)
					{
						m_SelectedControlPoint = point;
						m_SelectedControlPointOwner = entity;
						m_SelectionContext = entity;
						hit = true;
					}
					
				}
				if (!hit)
				{
					m_SelectedControlPoint = {};
					m_SelectedControlPointOwner = {};
				}
				
			}
		}
			});
	
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			if (IsBezierCurve(entity, &*m_Context))
			{
				
				
				BezierCurveComponent* bcc = static_cast<BezierCurveComponent*>(entity.GetComponent<NativeScriptComponent>().Instance);
				std::list<Entity>::iterator it = bcc->GetPoints().begin();
				while(it != bcc->GetPoints().end())
				{
					bool entityDeleted = false;
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
					flags |= ((m_SelectedControlPoint == *it) ? ImGuiTreeNodeFlags_Selected : 0);

					auto& tag = it->GetComponent<TagComponent>().Tag;
					bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)*it, flags, tag.c_str());
					if (ImGui::IsItemClicked())
					{
						m_SelectedControlPoint = *it;
						m_SelectedControlPointOwner = entity;
					}
					if (m_SelectedControlPoint == *it)
					{
						it->GetComponent<TransformComponent>().Scale = {8.0f, 8.0f, 1.0f};
						it->GetComponent<SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 0.5f };
					}
					else
					{
						it->GetComponent<TransformComponent>().Scale = { 5.0f, 5.0f, 1.0f };
						it->GetComponent<SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 1.0f };
					}
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete Point"))
							entityDeleted = true;

						ImGui::EndPopup();
					}
					if (opened)
						ImGui::TreePop();

					if (entityDeleted)
					{
						m_Context->DestroyEntity(*it);
						if (m_SelectedControlPoint == *it)
							m_SelectedControlPoint = {};
						it = bcc->GetPoints().erase(it);
						continue;
					}
					++it;
					
					
				}
			}
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (IsBezierCurve(entity, &*m_Context))
			{
				m_SelectedControlPoint = {};
				m_SelectedControlPointOwner = {};
				static_cast<BezierCurveComponent*>(m_SelectionContext.GetComponent<NativeScriptComponent>().Instance)->OnDestroy();
			}
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
			{
				m_SelectionContext = {};
			}
		}
	}

	

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunciton)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentReegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentReegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunciton(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();

		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				if (!m_SelectionContext.HasComponent<CameraComponent>())
					m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			// Curve editor added
			if (ImGui::MenuItem("Bezier Curve"))
			{
				if (!m_SelectionContext.HasComponent<NativeScriptComponent>())
					m_SelectionContext.AddComponent<NativeScriptComponent>().Bind<BezierCurveComponent>();
				ImGui::CloseCurrentPopup();
			}
			// ----------
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		bool isBezier = IsBezierCurve(entity, &*m_Context) && entity.HasComponent<TransformComponent>();
		TransformComponent tc;

		if (isBezier)
		{
			tc = entity.GetComponent<TransformComponent>();
		}

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{

				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});
		if (isBezier)
		{
			BezierCurveComponent* bcc= static_cast<BezierCurveComponent*>(entity.GetComponent<NativeScriptComponent>().Instance);
			TransformComponent* newtc = &entity.GetComponent<TransformComponent>();
				bcc->Translate(tc.Translation, newtc->Translation);
				bcc->Rotate(tc.Rotation, newtc->Rotation);
				bcc->Scale(tc.Scale, newtc->Scale);
		}

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

		ImGui::Checkbox("Primary", &component.Primary);

		const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
		int32_t currentProjectionTypeIndex = (int32_t)camera.GetProjectionType();
		if (ImGui::BeginCombo("Projection", projectionTypeStrings[currentProjectionTypeIndex]))
		{
			for (int32_t i = 0; i < 2; i++)
			{
				const bool isSelected = (currentProjectionTypeIndex == i);
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeIndex = i;
					camera.SetProjectionType((SceneCamera::ProjectionType)i);
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
		{
			float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
			if (ImGui::DragFloat("FOV", &verticalFOV))
				camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));
			float perspectiveNear = camera.GetPerspectiveNearClip();
			if (ImGui::DragFloat("Near Clip", &perspectiveNear))
				camera.SetPerspectiveNearClip(perspectiveNear);

			float perspectiveFar = camera.GetPerspectiveFarClip();
			if (ImGui::DragFloat("Far Clip", &perspectiveFar))
				camera.SetPerspectiveFarClip(perspectiveFar);


		}
		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
		{
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);
			float orthoNear = camera.GetOrthographicNearClip();
			if (ImGui::DragFloat("Near Clip", &orthoNear))
				camera.SetOrthographicNearClip(orthoNear);

			float orthoFar = camera.GetOrthographicFarClip();
			if (ImGui::DragFloat("Far Clip", &orthoFar))
				camera.SetOrthographicFarClip(orthoFar);

			ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
		}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			});
		// Added in Curve editor
		if (IsBezierCurve(entity, &*m_Context))
		{
			DrawComponent<NativeScriptComponent>("Bezier Curve Component", entity, [=](auto& component)
				{
					
					BezierCurveComponent* bcc = dynamic_cast<BezierCurveComponent*>(component.Instance);
					if (ImGui::Selectable("Cursor Mode", !bcc->DrawEnabled))
						bcc->DrawEnabled = false;
					if (ImGui::Selectable("Draw Mode", bcc->DrawEnabled))
						bcc->DrawEnabled = true;
				});

		}
		//--------------
	}
	void SceneHierarchyPanel::DrawControlPointComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}
		glm::vec3 coords = entity.GetComponent<TransformComponent>().Translation;
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
		glm::vec3 rotation = glm::degrees(component.Rotation);

			});
		if (coords != entity.GetComponent<TransformComponent>().Translation)
		{
			 static_cast<BezierCurveComponent*>(m_SelectedControlPointOwner.GetComponent<NativeScriptComponent>().Instance)->UpdateRelativeCoordinates();
		}
	}
}