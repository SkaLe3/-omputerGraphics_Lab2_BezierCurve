#include "ImageRendererComponent.h"

using namespace Engine;

void ImageRendererComponent::OnUpdate(Engine::Timestep ts)
{
	ImageComponent& ic = m_Entity.GetComponent<ImageComponent>();
	TransformComponent& tc = m_Entity.GetComponent<TransformComponent>();
	SpriteRendererComponent& src = m_Entity.GetComponent<SpriteRendererComponent>();

	tc.Scale.x = ic.Data[0].size();
	tc.Scale.y = ic.Data.size();

	if (!ic.Changed)
		return;


	// Ensure imageData is not empty
	if (!ic.Data.empty() && !ic.Data[0].empty()) {
		// Flatten the data and convert to uint32_t
		std::vector<uint32_t> flattenedData;
		flattenedData.reserve(ic.Data.size() * ic.Data[0].size());

		for (const auto& row : ic.Data) {
			for (const auto& pixel : row) {
				// Convert each component to uint8_t and pack into a uint32_t
				uint8_t r = static_cast<uint8_t>(pixel.r * 255.0f);
				uint8_t g = static_cast<uint8_t>(pixel.g * 255.0f);
				uint8_t b = static_cast<uint8_t>(pixel.b * 255.0f);
				uint8_t a = static_cast<uint8_t>(ic.Alpha * 255.0f);

				flattenedData.emplace_back((a << 24) | (b << 16) | (g << 8) | r);
			}
		}

		// Get a pointer to the data and the size of the data
		void* dataPtr = static_cast<void*>(flattenedData.data());
		std::size_t dataSize = flattenedData.size() * sizeof(uint32_t);
	src.Texture = Texture2D::Create(tc.Scale.x, tc.Scale.y);
	src.Texture->SetData(dataPtr, dataSize);
	ic.Changed = false;
	}
	
}
