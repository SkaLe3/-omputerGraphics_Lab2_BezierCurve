#pragma once
#include "Engine/Core/Core.h"


#include <glm/glm.hpp>

class EditorLayer;

class SettingsPanel
{
public:
	SettingsPanel() = default;
	SettingsPanel(EditorLayer* layer);


	void OnImGuiRender();



private:

	EditorLayer* m_Layer;

};