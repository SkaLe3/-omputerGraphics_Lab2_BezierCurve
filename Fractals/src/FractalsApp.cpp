#include <Engine.h>
#include <Engine/Core/EntryPoint.h>
#include "EditorLayer.h"


class Fractals : public Engine::Application
{
public:
	Fractals() : Engine::Application("Fractals")
	{
		PushLayer(new EditorLayer());
	}
	~Fractals()
	{

	}

private:



};

Engine::Application* Engine::CreateApplication()
{
	return new Fractals();
}