#pragma once
#include <Engine.h>

class CameraController : public Engine::ScriptableEntity
{
public:
	void OnCreate()
	{

	}

	void OnDestroy()
	{

	}

	void OnUpdate(Engine::Timestep ts)
	{
		using namespace Engine;
		auto& translation = GetComponent<Engine::TransformComponent>().Translation;
		auto& rotation = GetComponent<Engine::TransformComponent>().Rotation;


		if (Input::IsKeyPressed(Key::A)) {
			translation.x -= cos(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
			translation.y -= sin(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(Key::D)) {
			translation.x += cos(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
			translation.y += sin(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(Key::S)) {
			translation.x -= -sin(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
			translation.y -= cos(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(Key::W)) {
			translation.x += -sin(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
			translation.y += cos(glm::radians(rotation.z)) * m_CameraTranslationSpeed * ts;
		}

		m_CameraTranslationSpeed = GetComponent<CameraComponent>().Camera.GetOrthographicSize();
		//GetComponent<CameraComponent>().Camera.RecalculateProjection();
	}
	void OnEvent(Event& e)
	{
		using namespace Engine;
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::OnMouseScrolled));
	}


private:

	bool OnMouseScrolled(MouseScrolledEvent& e)
	{
		auto& camera = GetComponent<CameraComponent>().Camera;
		camera.SetOrthographicSize(std::max(camera.GetOrthographicSize() - e.GetYOffset() * 5.0f, 0.25f));
		return true;
	}
private:

	float m_CameraTranslationSpeed = 5.0f;
};