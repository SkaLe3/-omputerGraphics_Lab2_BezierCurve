#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Engine/Renderer/Camera.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
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

			float floatSeed = seed;
			glm::vec3 first_col = { floatSeed / (floatSeed + seed % 4 ? 30 : 20), floatSeed / (floatSeed + seed % 3 ? 30 : 5), floatSeed / (floatSeed + seed % 6 ? 20 : 15) };
			glm::vec3 second_col = { 0.9f, 0.9f, 0.9f };

			for (int32_t row = 0, height = Data.size(); row < height; ++row)
				for (int32_t col = 0, width = Data[0].size(); col < width; ++col)
					Data[row][col] = (row + col) * (col + seed) / (row+1) % 3 == 0 ? first_col : second_col;
		}
		ImageComponent(int32_t seed) : ImageComponent(160, 90, seed) {}

		std::vector<std::vector<glm::vec3>> Data;
		float Alpha = 1.0f;
	};
}