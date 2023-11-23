#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Camera.h"
#include "Shader.h"

namespace Engine {


	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResized(uint32_t width, uint32_t height);
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transfrom = glm::mat4(1.0f));
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}