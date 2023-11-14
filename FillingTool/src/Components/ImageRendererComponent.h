#pragma once

#include <Engine.h>


class ImageRendererComponent : public Engine::ScriptableEntity
{
public:
	ImageRendererComponent() {}
	void OnCreate() { m_Context = m_Entity.GetScene(); }
	void OnDestroy() { }
	void OnUpdate(Engine::Timestep ts);

private:
	Engine::Scene* m_Context;
};