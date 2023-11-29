#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Engine/Renderer/Camera.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Engine/Renderer/Texture.h"
namespace Engine {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};


	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

	};

	struct CameraComponent
	{
		Engine::SceneCamera Camera;
		bool Primary = true;  
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)(); 
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T());};
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}

	};
	struct ImageComponent
	{
	public:
		ImageComponent(int32_t width, int32_t height, int32_t seed) 
		{
			Data.resize(height);
			for (std::vector<glm::vec3>& row : Data)
				row.resize(width);

			float floatSeed = float(seed % 222) /  37.0f;
			glm::vec3 first_col = {glm::clamp(glm::sin(floatSeed),0.1f, 0.9f ),
				glm::clamp(glm::sin(floatSeed* floatSeed),0.1f, 0.9f),
				glm::clamp(glm::cos(floatSeed),0.1f, 0.9f)};

			glm::vec3 second_col = glm::vec3(1.0f) - first_col;

			for (int32_t row = 0, height = Data.size(); row < height; ++row)
				for (int32_t col = 0, width = Data[0].size(); col < width; ++col)
					Data[row][col] = glm::sin(row*col/(seed*seed)) * 10 > 3? first_col : second_col;
		}
		ImageComponent(int32_t seed) : ImageComponent(160, 90, seed) {}
		ImageComponent(const std::vector<std::vector<glm::vec3>>& image) : Data(image) {}

		std::vector<std::vector<glm::vec3>> Data;
		float Alpha = 1.0f;
		bool Changed = true;

		std::vector<std::vector<glm::vec2>> Polygons;
	};

	struct ControlPointComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		ControlPointComponent() = default;
		ControlPointComponent(const ControlPointComponent&) = default;
		ControlPointComponent(const glm::vec3& translation) : Translation(translation) {}
	};
}