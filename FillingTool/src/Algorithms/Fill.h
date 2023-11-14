#pragma once
#include <glm/glm.hpp>
#include <vector>

class Fill
{
public:
	static void FloodFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& targetColor, const glm::vec3& fillColor);
};