#pragma once

#include "Eis/Renderer/Renderer/RenderCommands.h"

#include "Eis/Renderer/Objects/Shader.h"
#include "Eis/Renderer/Objects/Texture.h"
#include "Eis/Renderer/Objects/OrthographicCamera.h"

// TOOD: renderer base class that the 2D and eventualy 3D renderer inherit

namespace Eis
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();


		// Primitives

		// Quads
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));


		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));


		// Circles
		static void DrawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawCircle(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawCircle(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

		
		// Lines
		static void DrawLine(const glm::vec2& start, const float& angle, const float& length, const glm::vec4& color, const float& width = 1.0f);
		static void DrawLine(const glm::vec3& start, const float& angle, const float& length, const glm::vec4& color, const float& width = 1.0f);
		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, const float& width = 1.0f);
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, const float& width = 1.0f);



		static void SetClearColor(const glm::vec4& col) { RenderCommands::SetClearColor(col); }
		static void Clear() { RenderCommands::Clear(); }
		static void OnWindowResized(uint32_t width, uint32_t height) { RenderCommands::SetViewport(0, 0, width, height); }
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}