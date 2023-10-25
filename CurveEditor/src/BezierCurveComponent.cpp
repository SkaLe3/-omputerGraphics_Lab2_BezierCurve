#include "BezierCurveComponent.h"

#include "Algorithms/Drawer.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace Engine;

glm::vec2 BezierCurveComponent::BezierFunction(const std::vector<glm::vec2>& points, float t)
{
	std::vector<glm::vec2> tempPoints = points;
	while (tempPoints.size() > 1)
	{
		std::vector<glm::vec2> newPoints;
		for (size_t i = 1; i < tempPoints.size(); i++)
		{
			glm::vec2 point = (1 - t) * tempPoints[i - 1] + t * tempPoints[i];
			newPoints.push_back(point);
		}
		tempPoints = newPoints;
	}
	return tempPoints[0];
}

void BezierCurveComponent::BezierCurve(const std::list<Entity>& entities, int32_t parts)
{
	if (entities.size() == 0)
		return;

	std::vector<glm::vec2> points;

	for (Entity ent : entities)
	{
		glm::vec3 trans = ent.GetComponent<TransformComponent>().Translation;
		points.push_back({ trans.x, trans.y });

	}

	glm::vec4 color = { 0.2f, 0.8f, 0.2f, 1.0f };
	if (m_Entity.HasComponent<SpriteRendererComponent>())
		color = GetComponent<SpriteRendererComponent>().Color;

	glm::vec2 prevPoint = points[0];
	for (float step = 1.0f / (float)parts, t = step; t <= 1.0f; t += step)
	{
		glm::vec2 point = BezierFunction(points, t);
		Drawer::BresenhamLine(prevPoint, point, color);
		prevPoint = point;
	}
	glm::vec2 point = BezierFunction(points, 1);
	Drawer::BresenhamLine(prevPoint, point, color);

	for (int i = 1, size = points.size(); i < size; i++)
	{
		Drawer::BresenhamLine(points[i - 1], points[i], { 1.0f, 1.0f, 1.0f, 0.1f }, 0.2f);
	}
	
}


void BezierCurveComponent::OnUpdate(Engine::Timestep ts)
{

	BezierCurve(m_ControlPoints, 100);
}

void BezierCurveComponent::Translate(const glm::vec3& oldTranslation, const glm::vec3& newTranslation)
{
	if (oldTranslation == newTranslation)
		return;
	glm::vec3 difference = newTranslation - oldTranslation;
	for (Entity ent : m_ControlPoints)
	{
		glm::vec3& pointTranslation = ent.GetComponent<TransformComponent>().Translation;
		pointTranslation += difference;
	}
}

void BezierCurveComponent::Rotate(const glm::vec3& oldRotation, const glm::vec3& newRotation)
{
	if (oldRotation == newRotation)
		return;
	glm::vec2 pivot = GetComponent<TransformComponent>().Translation;

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-pivot, 0.0f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), newRotation.z - oldRotation.z, { 0.0f, 0.0f, 1.0f });
	glm::mat4 transform = rotation *translation;

	for (Entity ent : m_ControlPoints)
	{
		glm::vec3& point = ent.GetComponent<TransformComponent>().Translation;
		point = glm::vec3(transform * glm::vec4(point, 1.0f));
	}

}

void BezierCurveComponent::Scale(const glm::vec3& oldScale, const glm::vec3& newScale)
{
	if (oldScale == newScale)
		return;

	for (Entity ent : m_ControlPoints)
	{
		glm::vec3& pointTranslation = ent.GetComponent<TransformComponent>().Translation;
		glm::vec3& curveTranslation = m_Entity.GetComponent<TransformComponent>().Translation;
		glm::vec3 vect = pointTranslation - curveTranslation;
		glm::vec3& relativeTranslation = ent.GetComponent<ControlPointComponent>().Translation;
		pointTranslation.x = curveTranslation.x + relativeTranslation.x * newScale.x;
		pointTranslation.y = curveTranslation.y + relativeTranslation.y * newScale.y;
	}
}

void BezierCurveComponent::UpdateRelativeCoordinates()
{
	for (Entity ent : m_ControlPoints)
	{
		glm::vec3& translation = ent.GetComponent<ControlPointComponent>().Translation;
		translation = ent.GetComponent<TransformComponent>().Translation - m_Entity.GetComponent<TransformComponent>().Translation;

	}
	m_Entity.GetComponent<TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
}


bool IsBezierCurve(Engine::Entity ent, Engine::Scene* scene)
{
	BezierCurveComponent* bezierCurveComponent;
	if (ent && ent.HasComponent<Engine::NativeScriptComponent>())
	{
		bezierCurveComponent = dynamic_cast<BezierCurveComponent*>(ent.GetComponent<Engine::NativeScriptComponent>().Instance);
		return bezierCurveComponent != nullptr;
	}
	return false;
}