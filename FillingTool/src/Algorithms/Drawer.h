#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
class Drawer
{
public:
	static void DDALine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color = {0.9f, 0.9f, 0.9f, 1.0f});
	static void BresenhamLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color = {0.9f, 0.9f, 0.9f, 1.0f}, float level = 0.0f);
	static void BresenhamLine(const std::vector<std::vector<glm::vec3>>,const glm::vec2& start, const glm::vec2& end, const glm::vec3& color = { 0.9f, 0.9f, 0.9f }, float level = 0.0f);
	static void BresenhamCircle(const glm::vec2& center, int32_t radius, const glm::vec4& color = { 0.9f, 0.9f, 0.9f, 1.0f });
	static void WuLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color = { 0.9f, 0.9f, 0.9f, 1.0f });

	static void DrawCircle(const glm::vec2& center, int32_t x, int32_t y, const glm::vec4& color = { 0.9f, 0.9f, 0.9f, 1.0f });


	static void BresenhamLineImage(std::vector<std::vector<glm::vec3>>& image, const glm::vec2& start, const glm::vec2& end, const glm::vec3& color = { 0.9f, 0.9f, 0.9f }, float level = 0.0f);

private:
	static int32_t IntPart(float x) { return glm::floor(x); }
	static float FracPart(float x) { return x - IntPart(x); }
	static float RevFracPart(float x) { return 1 - FracPart(x); }

};

