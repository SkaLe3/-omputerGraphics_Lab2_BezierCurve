#include "Fill.h"
#include <algorithm>
#include "Algorithms/Drawer.h"
#include <iostream>

bool Fill::FloodFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& targetColor, const glm::vec3& fillColor)
{
	int32_t height = image.size();
	int32_t width = image[0].size();

	if (targetColor == fillColor)
		return false;

	// Check boundaries
	if (x < 0 || x >= width || y < 0 || y >= height)
		return false;

	std::stack<std::pair<int32_t, int32_t>> pixelsToVisit;
	pixelsToVisit.push(std::make_pair(x, y));

	while (!pixelsToVisit.empty()) {
		auto pixel = pixelsToVisit.top();
		pixelsToVisit.pop();

		int32_t curX = pixel.first;
		int32_t curY = pixel.second;

		// Check for target color
		if (image[curY][curX] == targetColor) {
			// Set current pixel to the fill color
			image[curY][curX] = fillColor;

			// Visit adjacent pixels
			if (curX + 1 < width)
				pixelsToVisit.push(std::make_pair(curX + 1, curY)); // Right
			if (curX - 1 >= 0)
				pixelsToVisit.push(std::make_pair(curX - 1, curY)); // Left
			if (curY + 1 < height)
				pixelsToVisit.push(std::make_pair(curX, curY + 1)); // Down
			if (curY - 1 >= 0)
				pixelsToVisit.push(std::make_pair(curX, curY - 1)); // Up
		}
	}
	return true;
}

bool Fill::BoundaryFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& boundaryColor, const glm::vec3& fillColor)
{
	int32_t width = image[0].size();
	int32_t height = image.size();

	// Check boundaries
	if (x < 0 || x >= width || y < 0 || y >= height)
		return false;

	std::stack<std::pair<int32_t, int32_t>> pixelStack;
	pixelStack.push(std::make_pair(x, y));

	while (!pixelStack.empty()) {
		std::pair<int32_t, int32_t> currentPixel = pixelStack.top();
		pixelStack.pop();

		x = currentPixel.first;
		y = currentPixel.second;

		if (image[y][x] != boundaryColor && image[y][x] != fillColor) {
			image[y][x] = fillColor;

			// Push adjacent pixels onto the stack
			if (x + 1 < width)
				pixelStack.push(std::make_pair(x + 1, y)); // Right
			if (x - 1 >= 0)
				pixelStack.push(std::make_pair(x - 1, y)); // Left
			if (y + 1 < height)
				pixelStack.push(std::make_pair(x, y + 1)); // Down
			if (y - 1 >= 0)
				pixelStack.push(std::make_pair(x, y - 1)); // Up
		}
	}

	return true;
}
	

bool Fill::ScanlineFill(std::vector<std::vector<glm::vec3>>& image, const std::vector<glm::vec2>& vertices, const glm::vec3& fillColor)
{
	std::vector<ScanlineEdge> edges;

	int32_t numVertices = vertices.size();
	for (int32_t i = 0; i < numVertices; ++i)
	{
		int32_t x1 = vertices[i].x;
		int32_t y1 = vertices[i].y;
		int32_t x2 = vertices[(i + 1) % numVertices].x;
		int32_t y2 = vertices[(i + 1) % numVertices].y;

		if (y1 > y2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		
		if (y1 != y2)
		{
			float slope = static_cast<float>(x2 - x1) / static_cast<float>(y2 - y1);
			edges.emplace_back(y1, y2, x1, slope);

		}
	}

	// Sort the edges 
	std::sort(edges.begin(), edges.end(), [](const ScanlineEdge& a, const ScanlineEdge& b)
		{
			return a.ymin < b.ymin;
		});

	int32_t yMin = edges.front().ymin;
	int32_t yMax = edges.back().ymax;

	// Iterate over scanlines
	for (int32_t y = yMin; y <= yMax; ++y)
	{
		std::vector<int> intersections;

		// Find intersections of the scanline with each edge
		for (const ScanlineEdge& edge : edges)
		{
			if (y >= edge.ymin && y < edge.ymax)
			{
				int x = static_cast<int>(edge.x + edge.slope * (y - edge.ymin));
				intersections.push_back(x);
			}
		}

		// Sort intersections
		std::sort(intersections.begin(), intersections.end());


		// Fill between pairs of intersections
		for (size_t i = 0; i < intersections.size(); i += 2)
		{
			int32_t x1 = intersections[i];
			int32_t x2 = intersections[i + 1];
			Drawer::BresenhamLineImage(image, { x1, y }, { x2, y }, fillColor);
		}
	}
	return true;
}


bool Fill::SpanFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& fillColor, const glm::vec3& targetColor)
{
	std::vector<glm::ivec2> stack;
	stack.push_back({ x, y });

	while (!stack.empty())
	{
		glm::ivec2 current = stack.back();
		stack.pop_back();
		int lx = current.x;

		while (lx >= 0 && lx < image[0].size() && current.y >= 0 && current.y < image.size() && image[current.y][lx] == targetColor)
		{
			image[current.y][lx] = fillColor;
			--lx;
		}

		int rx = current.x + 1;
		while (rx >= 0 && rx < image[0].size() && current.y >= 0 && current.y < image.size() && image[current.y][rx] == targetColor)
		{
			image[current.y][rx] = fillColor;
			++rx;
		}

		scan(image, lx+1, rx-1, current.y + 1, stack, targetColor);
		scan(image, lx+1, rx-1, current.y - 1, stack, targetColor);
	}
	return true;
}

void scan(std::vector<std::vector<glm::vec3>>& image,int lx, int rx, int y, std::vector<glm::ivec2>& stack, const glm::vec3& targetColor)
{
	for (int i = lx; i < rx; i++) 
	{
		if (i >= 0 && i < image[0].size() && y >= 0 && y < image.size() && image[y][i] == targetColor) 
		{
			stack.push_back({i, y});
		}
	}
}


bool isPointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygon) {
	int crossings = 0;

	for (size_t i = 0; i < polygon.size(); ++i) {
		glm::vec2 v1 = polygon[i];
		glm::vec2 v2 = polygon[(i + 1) % polygon.size()];

		if ((v1.y <= point.y && point.y < v2.y) || (v2.y <= point.y && point.y < v1.y)) {
			float edgeX = (point.y - v1.y) / (v2.y - v1.y) * (v2.x - v1.x) + v1.x;
			if (point.x < edgeX) {
				crossings++;
			}
		}
	}

	return (crossings % 2 != 0);
}

std::vector<int> findPolygonsIndices(const glm::vec2& point, const std::vector<std::vector<glm::vec2>>& polygons) {
	std::vector<int> insidePolygons;

	for (size_t i = 0; i < polygons.size(); ++i) {
		if (isPointInPolygon(point, polygons[i])) {
			insidePolygons.push_back(static_cast<int>(i));
		}
	}

	return insidePolygons;
}

void DrawPolygons(std::vector<std::vector<glm::vec3>>& image, std::vector<std::vector<glm::vec2>>& polygons, const glm::vec3& color)
{
	for (std::vector<glm::vec2>& polygon : polygons)
	{
		for (size_t i = 0; i < polygon.size() - 1; ++i)
			Drawer::BresenhamLineImage(image, polygon[i], polygon[i + 1], color);
		Drawer::BresenhamLineImage(image, polygon[polygon.size() - 1], polygon[0], color);
	}


}
