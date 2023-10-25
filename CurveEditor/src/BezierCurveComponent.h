#pragma once

#include <Engine.h>


class BezierCurveComponent : public Engine::ScriptableEntity
{
public:
	
	BezierCurveComponent() {}
	void OnCreate() { m_Context = m_Entity.GetScene(); }
	void OnDestroy() {
		for (Engine::Entity ent : m_ControlPoints)
			m_Context->DestroyEntity(ent);
	}
	void OnUpdate(Engine::Timestep ts);

	void CreatePoint(const glm::vec3& coords = {0.0f, 0.0f, 0.0f})
	{
		Engine::Entity point = m_Context->CreateEntity(std::string("point" + std::to_string(m_PointCounter)));
		point.AddComponent<Engine::SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		point.AddComponent<Engine::ControlPointComponent>(coords - m_Entity.GetComponent<Engine::TransformComponent>().Translation);
		point.GetComponent<Engine::TransformComponent>().Scale = { 5.0f, 5.0f, 1.0f };
		point.GetComponent<Engine::TransformComponent>().Translation = coords;
		m_ControlPoints.push_back(point);
		m_PointCounter++;
	}
	std::list<Engine::Entity>& GetPoints() { return m_ControlPoints; }

	void Translate(const glm::vec3& oldTranslation, const glm::vec3& newTranslation);
	void Rotate(const glm::vec3& oldRotation, const glm::vec3& newRotation);
	void Scale(const glm::vec3& oldScale, const glm::vec3& newScale);

	void UpdateRelativeCoordinates();

private:
	glm::vec2 BezierFunction(const std::vector<glm::vec2>& points, float t);
	void BezierCurve(const std::list<Engine::Entity>& entities, int32_t parts);
public:
	bool DrawEnabled = false;
private:
	std::list<Engine::Entity> m_ControlPoints;
	Engine::Scene* m_Context;
	int32_t m_PointCounter = 0;
};



bool IsBezierCurve(Engine::Entity ent, Engine::Scene* scene);