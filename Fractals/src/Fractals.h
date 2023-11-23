#pragma once
#include <glm/glm.hpp>
#include <Engine.h>
#include <string>


class Fractal
{
public:
	Fractal()
	{
	}
	void CreateShader(const std::string& name)
	{
							
		m_Shader = Engine::Shader::Create(name);
	}

	Engine::Ref<Engine::Shader>& GetShader() { return m_Shader; }
	void Update(const glm::vec2& screenSize, const glm::vec2& translation, double zoom, int iterations) 
	{
		m_Shader->Bind();
		m_Shader->SetFloat("u_ScreenRatio", screenSize.x / screenSize.y);
		m_Shader->SetFloat2("u_ScreenSize", screenSize);
		m_Shader->SetFloat2("u_Center", translation);
		m_Shader->SetFloat("u_Zoom", zoom);
		m_Shader->SetInt("u_itr", iterations);
	}

private:

	Engine::Ref<Engine::Shader> m_Shader;
};
