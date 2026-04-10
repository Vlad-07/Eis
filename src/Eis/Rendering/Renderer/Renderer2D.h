#pragma once

#include "Eis/Rendering/Renderer/RenderCommands.h"

#include "Eis/Rendering/Objects/Texture.h"
#include "Eis/Rendering/Objects/OrthographicCamera.h"


namespace Eis
{
	class Shader;

	class Renderer2D
	{
	public:
	// Called by app

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Triangles
		// TODO: local verts

		static void DrawTriangle(const glm::mat3& worldVerts, const glm::vec4& color);
		static void DrawTriangle(const glm::mat3& worldVerts, const glm::mat3x4& colors);

		static void DrawRotatedTriangle(const glm::mat3& worldVerts, float rotation, const glm::vec4& color);
		static void DrawRotatedTriangle(const glm::mat3& worldVerts, float rotation, const glm::mat3x4& colors);


		// Quads

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));


		static void DrawQuad(const glm::mat4x3& worldVerts, const glm::vec4& color);
		static void DrawQuad(const glm::mat4x3& worldVerts, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));


		// Circles

		static void DrawCircle(const glm::vec2& position, float diameter, const glm::vec4& color, float thickness = 1.0f, float fade = 0.0f);
		static void DrawCircle(const glm::vec2& position, const glm::vec2& diameter, const glm::vec4& color, float thickness = 1.0f, float fade = 0.0f);
		static void DrawCircle(const glm::vec3& position, float diameter, const glm::vec4& color, float thickness = 1.0f, float fade = 0.0f);
		static void DrawCircle(const glm::vec3& position, const glm::vec2& diameter, const glm::vec4& color, float thickness = 1.0f, float fade = 0.0f);

		
		// Lines

		static void DrawLine(const glm::vec2& start, const float& angle, const float& length, const glm::vec4& color);
		static void DrawLine(const glm::vec3& start, const float& angle, const float& length, const glm::vec4& color);
		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);


	// Stats

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t TriangleCount = 0;
			uint32_t QuadCount = 0;
			uint32_t CircleCount = 0;
			uint32_t LineCount = 0;

			uint32_t VertexCount() const { return TriangleCount * 3 + (QuadCount + CircleCount) * 4 + LineCount * 2; }
			uint32_t IndexCount() const { return TriangleCount * 3 + (QuadCount + CircleCount) * 6 + LineCount * 2; }
		};

		static const Statistics& GetStats();
		static void ResetStats();

	// Commands

		static void SetLineWidth(float width);

		static void SetClearColor(const glm::vec3& col) { SetClearColor(glm::vec4(col, 1.0f)); }
		static void SetClearColor(const glm::vec4& col) { RenderCommands::SetClearColor(col); }

		static void Clear() { RenderCommands::Clear(); }

		// HACK: hijack quad shader for custom shaders
		// TODO: custom shader support
		static Ref<Shader> GetQuadShader();


	// Only called by engine

		static void Init();
		static void Shutdown();

		static void OnWindowResized(uint32_t width, uint32_t height) { RenderCommands::SetViewport(0, 0, width, height); }

	private:
		static void StartBatch();
		static void StartBatchTris();
		static void StartBatchQuads();
		static void StartBatchCircles();
		static void StartBatchLines();

		static void Flush();
		static void FlushTris();
		static void FlushQuads();
		static void FlushCircles();
		static void FlushLines();

		static void NextBatchTris();
		static void NextBatchQuads();
		static void NextBatchCircles();
		static void NextBatchLines();

		static float GetTextureIndex(const Ref<Texture2D>& texture);
		static float PushTexture(const Ref<Texture2D>& texture); // Returns texture's new index
	};
}