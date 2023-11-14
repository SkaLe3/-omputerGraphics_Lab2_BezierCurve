#include <Engine.h>
#include <Engine/Core/EntryPoint.h>
#include "EditorLayer.h"


class FillingTool : public Engine::Application
{
public:
	FillingTool() : Engine::Application("Editor")
	{
		PushLayer(new EditorLayer());
	}
	~FillingTool()
	{

	}

private:



};

Engine::Application* Engine::CreateApplication()
{
	return new FillingTool();
}