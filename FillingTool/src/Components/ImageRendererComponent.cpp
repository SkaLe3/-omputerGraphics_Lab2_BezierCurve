#include "ImageRendererComponent.h"

using namespace Engine;

void ImageRendererComponent::OnUpdate(Engine::Timestep ts)
{
	ImageComponent& ic = m_Entity.GetComponent<ImageComponent>();
	TransformComponent& tc = m_Entity.GetComponent<TransformComponent>();

	for (int32_t row = 0, height = ic.Data.size(); row < height; ++row)
		for (int32_t col = 0, width = ic.Data[0].size(); col < width; ++col)
			Renderer2D::DrawQuad(glm::vec3((int)tc.Translation.x + col + 0.5 ,(int)tc.Translation.y - row - 0.5,(int)tc.Translation.z ), { 1.0f, 1.0f }, glm::vec4(ic.Data[row][col], ic.Alpha));
			//Renderer2D::DrawQuad(glm::vec3((int)tc.Translation.x + col, (int)tc.Translation.y + row, (int)tc.Translation.z), { 1.0f, 1.0f }, {0.9f, 0.9f, 0.9f, 1.0f});

}
