#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <stack>
#include "Algorithms/Drawer.h"

struct ScanlineEdge 
{
	int32_t ymin, ymax;
	float x, slope;

	ScanlineEdge(int32_t ymin, int32_t ymax, float x, float slope) : ymin(ymin), ymax(ymax), x(x), slope(slope) {}
};



class Fill
{
public:
	static bool FloodFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& targetColor, const glm::vec3& fillColor);
	static bool BoundaryFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& boundaryColor, const glm::vec3& fillColor);
	static bool ScanlineFill(std::vector<std::vector<glm::vec3>>& image, const std::vector<glm::vec2>& vertices, const glm::vec3& fillColor);
	static bool SpanFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& fillColor, const glm::vec3& targetColor);
};
void scan(std::vector<std::vector<glm::vec3>>& image,int lx, int rx, int y, std::vector<glm::ivec2>& stack, const glm::vec3& targetColor);
bool isPointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygon);
std::vector<int> findPolygonsIndices(const glm::vec2& point, const std::vector<std::vector<glm::vec2>>& polygons);
void DrawPolygons(std::vector<std::vector<glm::vec3>>& image, std::vector<std::vector<glm::vec2>>& polygons, const glm::vec3& color);