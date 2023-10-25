#include <Engine.h>
#include <Engine/Core/EntryPoint.h>
#include "EditorLayer.h"


class CurveEditor : public Engine::Application
{
public:
	CurveEditor() : Engine::Application("Editor")
	{
		PushLayer(new EditorLayer());
	}
	~CurveEditor()
	{

	}

private:



};

Engine::Application* Engine::CreateApplication()
{
	return new CurveEditor();
}