#pragma once

#include "Eis/Rendering/Renderer/RenderCommands.h"

#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/Texture.h"
#include "Eis/Rendering/Objects/OrthographicCamera.h"


namespace Eis
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void StartBatch();
		static void StartBatchQuads();
		static void StartBatchCircles();
		static void StartBatchLines();

		static void NextBatchQuads();
		static void NextBatchCircles();
		static void NextBatchLines();

		static void Flush();
		static void FlushQuads();
		static void FlushCircles();
		static void FlushLines();

		// Primitives

		// Quads
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));


		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));


		// Circles
		static void DrawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float fade = 0.0f);
		static void DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float fade = 0.0f);

		
		// Lines
		static void DrawLine(const glm::vec2& start, const float& angle, const float& length, const glm::vec4& color);
		static void DrawLine(const glm::vec3& start, const float& angle, const float& length, const glm::vec4& color);
		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);


		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t CircleCount = 0;
			uint32_t LineCount = 0;

			uint32_t VerexCount() const { return (QuadCount + CircleCount) * 4.0f + LineCount * 2.0f; }
			uint32_t IndexCount() const { return (QuadCount + CircleCount) * 6.0f + LineCount * 2.0f; }
		};

		static const Statistics& GetStats();
		static void ResetStats();

		// Commands
		static float GetLineWidth();
		static void  SetLineWidth(float width);

		static void SetClearColor(const glm::vec4& col) { RenderCommands::SetClearColor(col); }
		static void Clear() { RenderCommands::Clear(); }
		static void OnWindowResized(uint32_t width, uint32_t height) { RenderCommands::SetViewport(0, 0, width, height); }
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}