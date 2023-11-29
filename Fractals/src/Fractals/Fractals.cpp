#include "Fractals.h"


KochSnowfkale::KochSnowfkale()
{
	m_Shader = Engine::Shader::Create("assets/shaders/KochSnowflake.glsl");
}

void KochSnowfkale::Update(const glm::vec2& screenSize, const glm::vec2& translation, double zoom, int iterations, const glm::vec3& color)
{
	m_Shader->Bind();
	m_Shader->SetFloat2("u_ScreenSize", screenSize);
	m_Shader->SetFloat2("u_Center", translation);
	m_Shader->SetFloat("u_Zoom", zoom);
	m_Shader->SetInt("u_itr", iterations);
	m_Shader->SetFloat3("u_Color", color);
}

MandelbrotSet::MandelbrotSet()
{
	m_Shader = Engine::Shader::Create("assets/shaders/Mandelbrot.glsl");
}

void MandelbrotSet::Update(const glm::vec2& screenSize, const glm::vec2& translation, double zoom, int iterations)
{
	m_Shader->Bind();
	m_Shader->SetFloat2("u_ScreenSize", screenSize);
	m_Shader->SetFloat2("u_Center", translation);
	m_Shader->SetFloat("u_Zoom", zoom);
	m_Shader->SetInt("u_itr", iterations);
}

BarnsleyFern::BarnsleyFern()
{
	for (int32_t i = 0; i < 32; i++)
		samplers[i] = i;

	m_Shader = Engine::Shader::Create("assets/shaders/BarnsleyFern.glsl");
}

void BarnsleyFern::Update(const glm::vec2& screenSize, const glm::vec3& color)
{
	m_Shader->Bind();
	m_Shader->SetFloat2("u_ScreenSize", screenSize);
	m_Shader->SetFloat3("u_Color", color);
	m_Shader->SetIntArray("u_Textures", samplers, 32);
}
