#pragma once
#include <Engine.h>

class CameraController
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
		const float d = 0.01 / zoom;

		if (Input::IsKeyPressed(Key::A)) {
			translation.x -= d;
		}
		if (Input::IsKeyPressed(Key::D)) {
			translation.x += d;
		}

		if (Input::IsKeyPressed(Key::S)) {
			translation.y -= d;
		}
		if (Input::IsKeyPressed(Key::W)) {
			translation.y += d;
		}
	}

	void OnEvent(Engine::Event& e)
	{
		using namespace Engine;
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::OnMouseScrolled));
	}


private:

	bool OnMouseScrolled(Engine::MouseScrolledEvent& e)
	{
		zoom += e.GetYOffset() * 0.02 * zoom;
		if (zoom < 0.1)
		{
			zoom = 0.1;
		}

		return true;
	}
public:
	glm::vec2 translation = { 0.0f, 0.0f };
	double zoom = 0.8f;
};