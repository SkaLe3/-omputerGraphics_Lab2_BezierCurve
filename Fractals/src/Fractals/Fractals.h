#pragma once
#include <glm/glm.hpp>
#include <Engine.h>
#include <string>


class Fractal
{
public:
	Fractal() = default;
	virtual ~Fractal() = default;
	Engine::Ref<Engine::Shader>& GetShader() { return m_Shader; }

protected:
	Engine::Ref<Engine::Shader> m_Shader;
};

class KochSnowfkale : public Fractal
{
public:
	KochSnowfkale();
	void Update(const glm::vec2& screenSize, const glm::vec2& translation, double zoom, int iterations, const glm::vec3& color);
};

class MandelbrotSet : public Fractal
{
public:
	MandelbrotSet();
	void Update(const glm::vec2& screenSize, const glm::vec2& translation, double zoom, int iterations);
};

class BarnsleyFern : public Fractal
{
public:
	BarnsleyFern();
	void Update(const glm::vec2& screenSize, const glm::vec3& color);

private:
	int32_t samplers[32];
};