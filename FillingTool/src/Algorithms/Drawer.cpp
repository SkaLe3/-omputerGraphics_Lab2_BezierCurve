#include "Drawer.h"
#include <Engine.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine;
void Drawer::DDALine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
{

	float dx = glm::round(end.x) - glm::round(start.x);
	float dy = glm::round(end.y) - glm::round(start.y);

	int32_t step;

	if (glm::abs(dx) > abs(dy))
		step = abs(dx);
	else
		step = abs(dy);

	float xIncrement = dx / step;
	float yIncrement = dy / step;

	glm::vec2 current = start;

	for (int32_t i = 0; i <= step; i++)
	{
		Renderer2D::DrawPixel(glm::round(current), color);
		current.x += xIncrement;
		current.y += yIncrement;
	}
}

void Drawer::BresenhamLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float level)
{
	glm::vec2 _start = { glm::round(start.x), glm::round(start.y)};
	glm::vec2 _end = { glm::round(end.x), glm::round(end.y)};


	float dx = glm::abs(_end.x - _start.x);
	glm::vec2 s;
	if (start.x < end.x)
		s.x = 1;
	else
		s.x = -1;

	float dy = -glm::abs(_end.y - _start.y);
	if (_start.y < _end.y)
		s.y = 1;
	else
		s.y = -1;

	float err = dx + dy;

	

	float err2;
	while (true)
	{
		Renderer2D::DrawPixel({_start.x, _start.y}, color);
		if (_start.x == _end.x && _start.y == _end.y)
			break;
		err2 = 2 * err;
		if (err2 >= dy)
		{
			if (_start.x == _end.x)
				break;
			err += dy;
			_start.x += s.x;
		}
		if (err2 <= dx)
		{
			if (_start.y == _end.y)
				break;
			err += dx;
			_start.y += s.y;
		}
	}

}

void Drawer::BresenhamCircle(const glm::vec2& center, int32_t radius, const glm::vec4& color /*= { 0.9f, 0.9f, 0.9f, 1.0f }*/)
{
	int32_t x = 0;
	int32_t y = radius;
	int32_t d = 3 - 2 * radius;

	glm::vec2 _center = { glm::round(center.x), glm::round(center.y) };

	while (y >= x)
	{
		Drawer::DrawCircle(_center, x, y, color);
		
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		x++;
	}
}


void Drawer::WuLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color /*= { 0.9f, 0.9f, 0.9f, 1.0f }*/)
{
	glm::vec2 _start = { glm::round(start.x), glm::round(start.y)};
	glm::vec2 _end = { glm::round(end.x), glm::round(end.y) };
	bool steep = glm::abs(_end.y - _start.y) > glm::abs(_end.x - _start.x);

	if (steep)
	{
		_start = { _start.y, start.x };
		_end = { _end.y, _end.x };
	}
	if (_start.x > _end.x)
	{
		glm::vec2 temp = _start;
		_start = _end;
		_end = temp;
	}

	float dx = _end.x - _start.x;
	float dy = _end.y - _start.y;

	float gradient;

	if (dx == 0)
		gradient = 1.0;
	else
		gradient = dy / dx;

	int32_t xend = glm::round(_start.x);
	int32_t yend = _start.y + gradient * (xend - _start.x);
	int32_t xgap = RevFracPart(_start.x + 0.5f);
	int32_t xpxl1 = xend;
	int32_t ypxl1 = IntPart(yend);
	if (steep)
	{
		Renderer2D::DrawPixel({ ypxl1, xpxl1 }, { color.x, color.y, color.z, RevFracPart(yend) * xgap * color.w });
		Renderer2D::DrawPixel({ ypxl1+1, xpxl1 }, { color.x, color.y, color.z, FracPart(yend) * xgap * color.w });
	}
	else
	{
		Renderer2D::DrawPixel({ xpxl1, ypxl1 }, { color.x, color.y, color.z, RevFracPart(yend) * xgap * color.w });
		Renderer2D::DrawPixel({ xpxl1, ypxl1+1}, { color.x, color.y, color.z, FracPart(yend) * xgap * color.w });
	}

	float intery = yend + gradient;

	xend = glm::round(_end.x);
	yend = _end.y + gradient * (xend - 1);
	xgap = FracPart(_end.x + 0.5f);
	int32_t xpxl2 = xend;
	int32_t ypxl2 = IntPart(yend);
	if (steep)
	{
		Renderer2D::DrawPixel({ ypxl2, xpxl2 }, { color.x, color.y, color.z, RevFracPart(yend) * xgap * color.w });
		Renderer2D::DrawPixel({ ypxl2 + 1, xpxl2 }, { color.x, color.y, color.z, FracPart(yend) * xgap * color.w });
	}
	else
	{
		Renderer2D::DrawPixel({ xpxl2, ypxl2 }, { color.x, color.y, color.z, RevFracPart(yend) * xgap * color.w });
		Renderer2D::DrawPixel({ xpxl2, ypxl2 + 1}, { color.x, color.y, color.z, FracPart(yend) * xgap * color.w });
	}

	if (steep)
	{
		for (int32_t x = xpxl1 + 1; x <= xpxl2; x++)
		{
			Renderer2D::DrawPixel({ IntPart(intery), x }, { color.x, color.y, color.z, RevFracPart(intery) * color.w });
			Renderer2D::DrawPixel({ IntPart(intery) + 1, x }, { color.x, color.y, color.z, FracPart(intery) * color.w });
			intery = intery + gradient;
		}
	}
	else
	{
		for (int32_t x = xpxl1 + 1; x <= xpxl2; x++)
		{
			Renderer2D::DrawPixel({ x, IntPart(intery) }, { color.x, color.y, color.z, RevFracPart(intery) * color.w });
			Renderer2D::DrawPixel({ x, IntPart(intery) + 1 }, { color.x, color.y, color.z, FracPart(intery) * color.w });
			intery = intery + gradient;
		}
	}
}

void Drawer::DrawCircle(const glm::vec2& center, int32_t x, int32_t y, const glm::vec4& color)
{
	Renderer2D::DrawPixel({ center.x + x, center.y + y }, color);
	Renderer2D::DrawPixel({ center.x - x, center.y + y }, color);
	Renderer2D::DrawPixel({ center.x + x, center.y - y }, color);
	Renderer2D::DrawPixel({ center.x - x, center.y - y }, color);
	Renderer2D::DrawPixel({ center.x + y, center.y + x }, color);
	Renderer2D::DrawPixel({ center.x - y, center.y + x }, color);
	Renderer2D::DrawPixel({ center.x + y, center.y - x }, color);
	Renderer2D::DrawPixel({ center.x - y, center.y - x }, color);
}


