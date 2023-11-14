#include "Fill.h"
#include <stack>

void Fill::FloodFill(std::vector<std::vector<glm::vec3>>& image, int32_t x, int32_t y, const glm::vec3& targetColor, const glm::vec3& fillColor)
{
	int32_t height = image.size();
	int32_t width = image[0].size();

	if (targetColor == fillColor)
		return;

	// Check boundaries
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;

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
}

