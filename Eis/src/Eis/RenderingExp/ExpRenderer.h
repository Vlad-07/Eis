#pragma once

#include "Eis/Rendering/Renderer/RenderCommands.h"

#include "Eis/Rendering/Objects/Texture.h"
#include "Eis/Rendering/Objects/OrthographicCamera.h"


namespace Eis
{
	class Shader;

	class ExpRenderer
	{
	public:
		// Called by app

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Quads

		struct QuadDesc
		{
			glm::vec2 Pos;
			glm::vec2 Size;
			float Rotation;
			const Ref<Texture2D>& Tex;
			glm::vec4 Tint{ 1.0f };
			float Tiling{ 1.0f };
		};

		static void DrawQuad(const QuadDesc& desc);

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


		struct Light
		{
			glm::vec2 Position;
			glm::vec3 Color;
			float Range;
			float Intensity;
			float Volume;
		};
		// Call outside of scene!
		static void AddLight(const Light& light);

		static void SetAmbientLight(glm::vec3 color, float intensity);


		static void TmpLi(float infl);

		// Commands


		static void Init();
		static void Shutdown();

		static void OnWindowResized(uint32_t width, uint32_t height);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static void StartBatch();
		static void StartBatchQuads();

		static void Flush();
		static void FlushQuads();

		static void NextBatchQuads();

		static float GetTextureIndex(const Ref<Texture2D>& texture);
		static float PushTexture(const Ref<Texture2D>& texture); // Returns texture's new index
	};
}