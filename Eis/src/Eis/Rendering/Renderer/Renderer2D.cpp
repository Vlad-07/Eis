#include "Eispch.h"
#include "Renderer2D.h"

#include "Eis/Rendering/Renderer/RenderCommands.h"
#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/VertexArray.h"
#include "Eis/Rendering/Objects/VertexBufferData.h"
#include "Eis/Rendering/Objects/Camera.h"
#include "Eis/Rendering/Objects/EditorCamera2.h"

#include "Eis/Assets/Importers/ShaderImporter.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


namespace Eis
{
	struct TriangleVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};


	struct Renderer2DData
	{
		static constexpr uint32_t MaxTriangles   = 10'000;
		static constexpr uint32_t MaxTriVertices = MaxTriangles * 3;
		static constexpr uint32_t MaxTriIndices  = MaxTriangles * 3;

		static constexpr uint32_t MaxQuads        = 10'000;
		static constexpr uint32_t MaxQuadVertices = MaxQuads * 4;
		static constexpr uint32_t MaxQuadIndices  = MaxQuads * 6;

		static constexpr uint32_t MaxCircles        = 10'000;
		static constexpr uint32_t MaxCircleVertices = MaxCircles * 4;
		static constexpr uint32_t MaxCircleIndices  = MaxCircles * 6;

		static constexpr uint32_t MaxLines        = 10'000;
		static constexpr uint32_t MaxLineVertices = MaxLines * 2;

	#ifndef EIS_PLATFORM_WEB
		static constexpr uint8_t MaxTextureSlots = 32; // TODO: check max tex slots at runtime
	#else
		static constexpr uint8_t MaxTextureSlots = 16;
	#endif

		Ref<Shader> ColorShader;

		Ref<VertexArray> TriangleVertexArray;
		Ref<VertexBuffer> TriangleVertexBuffer;

		uint32_t TriangleIndexCount = 0;
		TriangleVertex* TriangleVertexBufferBase = nullptr;
		TriangleVertex* TriangleVertexBufferPtr = nullptr;


		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;

		uint32_t QuadVertexCount{};
		VertexBufferData QuadVertexBufferData;

		glm::mat4 QuadVertexPositions{};
		glm::mat4x2 QuadVertexTexCoords{};


		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;


		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;

		uint32_t LineIndexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 1.5f;


		Ref<Texture2D> WhiteTexture;

		uint16_t TextureSlotIndex = 1; // 0 is WhiteTex
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		struct CameraData
		{
			glm::mat4 ViewProjection{};
		} CameraBuf{};
		Ref<UniformBuffer> CameraUniformBuf;

		Renderer2D::Statistics Stats;
	};
	static Renderer2DData s_Data;


	void Renderer2D::Init()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		RenderCommands::Init();

		// Init Shaders

		int samplers[s_Data.MaxTextureSlots];
		for (uint8_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.ColorShader = ShaderImporter::LoadShader("resources/shaders/Color.glsl");

		s_Data.QuadShader = ShaderImporter::LoadShader("resources/shaders/Quad.glsl");
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

	//	s_Data.CircleShader = ShaderImporter::LoadShader("resources/shaders/Circle.glsl");


		// Init Triangles

		s_Data.TriangleVertexBuffer = VertexBuffer::Create(s_Data.MaxTriVertices * sizeof(TriangleVertex));
		s_Data.TriangleVertexBuffer->SetLayout(s_Data.ColorShader->GetAttributeLayout());

		s_Data.TriangleVertexArray = VertexArray::Create();
		s_Data.TriangleVertexArray->AddVertexBuffer(s_Data.TriangleVertexBuffer);

		uint32_t* triIndices = new uint32_t[s_Data.MaxTriVertices];
		for (uint32_t i = 0; i < s_Data.MaxTriIndices; i++)
			triIndices[i] = i;

		Ref<IndexBuffer> triIb = IndexBuffer::Create(triIndices, s_Data.MaxTriIndices);
		s_Data.TriangleVertexArray->SetIndexBuffer(triIb);
		delete[] triIndices;

		s_Data.TriangleVertexBufferBase = new TriangleVertex[s_Data.MaxTriVertices];


		// Init Quads

		const AttributeLayout& quadLayout = s_Data.QuadShader->GetAttributeLayout();
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadVertices * quadLayout.Stride);
		s_Data.QuadVertexBuffer->SetLayout(quadLayout);
		s_Data.QuadVertexBufferData.SetLayout(quadLayout, s_Data.MaxQuadVertices);

		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		uint32_t* quadIndices = new uint32_t[s_Data.MaxQuadIndices];
		for (uint32_t i = 0, off = 0; i < s_Data.MaxQuadIndices; i += 6)
		{
			quadIndices[i + 0] = off + 0;
			quadIndices[i + 1] = off + 1;
			quadIndices[i + 2] = off + 2;

			quadIndices[i + 3] = off + 2;
			quadIndices[i + 4] = off + 3;
			quadIndices[i + 5] = off + 0;

			off += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxQuadIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;



		// Init Circles

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxCircleVertices * sizeof(CircleVertex));
	//	s_Data.CircleVertexBuffer->SetLayout(s_Data.CircleShader->GetAttributeLayout());

		s_Data.CircleVertexArray = VertexArray::Create();
	//	s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // HACK: reuse quadIb

		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxCircleVertices];


		// Init Lines

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxLineVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout(s_Data.ColorShader->GetReflection().VertexAttributes);

		s_Data.LineVertexArray = VertexArray::Create();
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxLineVertices];


		// Init WhiteTexture

		const uint32_t whiteTexData = 0xffffffff;
		Buffer data{ static_cast<const void*>(&whiteTexData), sizeof(whiteTexData) };
		s_Data.WhiteTexture = Texture2D::Create(TextureSpec{}, data);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		s_Data.TextureSlotIndex = 1;


		// Init QuadVertex
		s_Data.QuadVertexPositions[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPositions[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);

		s_Data.QuadVertexTexCoords[0] = { 0.0f, 0.0f };
		s_Data.QuadVertexTexCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadVertexTexCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadVertexTexCoords[3] = { 0.0f, 1.0f };

		// Init camera data
		s_Data.CameraUniformBuf = UniformBuffer::Create(sizeof(s_Data.CameraBuf), 0);
	}

	void Renderer2D::Shutdown()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		delete[] s_Data.CircleVertexBufferBase;
		delete[] s_Data.LineVertexBufferBase;
	}


	void Renderer2D::BeginScene(const EditorCamera2& camera)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data.CameraBuf.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuf->SetData(&s_Data.CameraBuf, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data.CameraBuf.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuf->SetData(&s_Data.CameraBuf, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		Flush();
	}


	void Renderer2D::StartBatch()
	{
		StartBatchTris();
		StartBatchQuads();
		StartBatchCircles();
		StartBatchLines();
	}

	void Renderer2D::StartBatchTris()
	{
		s_Data.TriangleIndexCount = 0;
		s_Data.TriangleVertexBufferPtr = s_Data.TriangleVertexBufferBase;
	}
	void Renderer2D::StartBatchQuads()
	{
		s_Data.QuadVertexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}
	void Renderer2D::StartBatchCircles()
	{
		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
	}
	void Renderer2D::StartBatchLines()
	{
		s_Data.LineIndexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
	}


	void Renderer2D::Flush()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		FlushTris();
		FlushQuads();
		FlushCircles();
		FlushLines();
	}

	void Renderer2D::FlushTris()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.TriangleIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TriangleVertexBufferPtr - (uint8_t*)s_Data.TriangleVertexBufferBase);
		s_Data.TriangleVertexBuffer->SetData(s_Data.TriangleVertexBufferBase, dataSize);

		s_Data.ColorShader->Bind();
		RenderCommands::DrawIndexed(s_Data.TriangleVertexArray, s_Data.TriangleIndexCount);

		s_Data.Stats.DrawCalls++;
	}
	void Renderer2D::FlushQuads()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.QuadVertexCount == 0)
			return;

		const uint32_t dataSize = s_Data.QuadVertexBufferData.GetVertexDataSize() * s_Data.QuadVertexCount;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferData.GetData(), dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		const uint32_t indexCount = static_cast<uint32_t>(s_Data.QuadVertexCount * 1.5f);

		s_Data.QuadShader->Bind();
		RenderCommands::DrawIndexed(s_Data.QuadVertexArray, indexCount);

		s_Data.Stats.DrawCalls++;
	}
	void Renderer2D::FlushCircles()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.CircleIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
		s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

		s_Data.CircleShader->Bind();
		RenderCommands::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);

		s_Data.Stats.DrawCalls++;
	}
	void Renderer2D::FlushLines()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.LineIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

		s_Data.ColorShader->Bind();
		RenderCommands::SetLineWidth(s_Data.LineWidth);
		RenderCommands::DrawLines(s_Data.LineVertexArray, s_Data.LineIndexCount);

		s_Data.Stats.DrawCalls++;
	}


	void Renderer2D::NextBatchTris()
	{
		FlushTris();
		StartBatchTris();
	}
	void Renderer2D::NextBatchQuads()
	{
		FlushQuads();
		StartBatchQuads();
	}
	void Renderer2D::NextBatchCircles()
	{
		FlushCircles();
		StartBatchCircles();
	}
	void Renderer2D::NextBatchLines()
	{
		FlushLines();
		StartBatchLines();
	}



	void Renderer2D::DrawTriangle(const glm::mat3& worldVerts, const glm::vec4& color)
	{
		DrawTriangle(worldVerts, glm::mat3x4(color, color, color));
	}
	void Renderer2D::DrawTriangle(const glm::mat3& worldVerts, const glm::mat3x4& colors)
	{
		DrawRotatedTriangle(worldVerts, 0.0f, colors);
	}
	void Renderer2D::DrawRotatedTriangle(const glm::mat3& worldVerts, float rotation, const glm::vec4& color)
	{
		DrawRotatedTriangle(worldVerts, rotation, glm::mat3x4(color, color, color));
	}
	void Renderer2D::DrawRotatedTriangle(const glm::mat3& worldVerts, float rotation, const glm::mat3x4& colors)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.TriangleIndexCount >= s_Data.MaxTriVertices)
			NextBatchTris();

		glm::mat4 transform{ 1.0f };
		if (rotation != 0.0f)
		{
			const glm::vec3 center = (worldVerts[0] + worldVerts[1] + worldVerts[2]) / 3.0f;
			transform = glm::translate(glm::mat4(1.0f), center)
						* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
						* glm::translate(glm::mat4(1.0f), -center);
		}
		for (uint8_t i = 0; i < 3; i++)
		{
			s_Data.TriangleVertexBufferPtr->Position = transform * glm::vec4(worldVerts[i], 1.0f);
			s_Data.TriangleVertexBufferPtr->Color = colors[i];

			s_Data.TriangleVertexBufferPtr++;
		}

		s_Data.TriangleIndexCount += 3;

		s_Data.Stats.TriangleCount++;
	}



	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, nullptr, 1.0f, color);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, texture, tiling, tint);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		DrawRotatedQuad(position, size, 0.0f, texture, tiling, tint);
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(position, size, rotation, nullptr, 1.0f, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, texture, tiling, tint);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		const glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, position)
									* (rotation != 0.0f ? glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotation), glm::vec3{ 0.0f, 0.0f, 1.0f }) : 1.0f)
									* glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tint, tiling);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entityId)
	{
		DrawQuad(transform, nullptr, color, 1.0f, entityId);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint, float tiling, int32_t entityId)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const glm::mat4 verts{ transform * s_Data.QuadVertexPositions };

		if (s_Data.QuadVertexCount >= s_Data.MaxQuadIndices)
			NextBatchQuads();

		const float textureIndex = GetTextureIndex(texture);
		for (uint8_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferData.Set(s_Data.QuadVertexCount, AttribSemantic::Position, glm::vec3{ verts[i] });
			s_Data.QuadVertexBufferData.Set(s_Data.QuadVertexCount, AttribSemantic::Color, tint);
			s_Data.QuadVertexBufferData.Set(s_Data.QuadVertexCount, AttribSemantic::TexCoord0, s_Data.QuadVertexTexCoords[i]);
			s_Data.QuadVertexBufferData.Set(s_Data.QuadVertexCount, AttribSemantic::TexIndex, textureIndex);
			s_Data.QuadVertexBufferData.Set(s_Data.QuadVertexCount, AttribSemantic::TilingFactor, tiling);
			s_Data.QuadVertexBufferData.Set(s_Data.QuadVertexCount, AttribSemantic::EntityId, entityId);

			s_Data.QuadVertexCount++;
		}

		s_Data.Stats.QuadCount++;
	}



	void Renderer2D::DrawCircle(const glm::vec2& position, float diameter, const glm::vec4& color, float thickness, float fade)
	{
		DrawCircle(glm::vec3(position, 0.0f), glm::vec2(diameter), color, thickness, fade);
	}
	void Renderer2D::DrawCircle(const glm::vec2& position, const glm::vec2& diameter, const glm::vec4& color, float thickness, float fade)
	{
		DrawCircle(glm::vec3(position, 0.0f), diameter, color, thickness, fade);
	}
	void Renderer2D::DrawCircle(const glm::vec3& position, float diameter, const glm::vec4& color, float thickness, float fade)
	{
		DrawCircle(position, glm::vec2(diameter), color, thickness, fade);
	}
	void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec2& diameter, const glm::vec4& color, float thickness, float fade)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.CircleIndexCount >= s_Data.MaxCircleIndices)
			NextBatchCircles();

		const glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, position)
									* glm::scale(glm::mat4{ 1.0f }, { diameter.x, diameter.y, 1.0f });

		for (uint8_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;

			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.CircleCount++;
	}



	void Renderer2D::DrawLine(const glm::vec2& start, const float& angle, const float& length, const glm::vec4& color)
	{
		DrawLine(glm::vec3(start, 1.0f), angle, length, color);
	}
	void Renderer2D::DrawLine(const glm::vec3& start, const float& angle, const float& length, const glm::vec4& color)
	{
		const glm::vec3 offset = glm::rotate(glm::vec3(0.0f, length, 0.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		const glm::vec3 end = start + offset;
		DrawLine(start, end, color);
	}
	void Renderer2D::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
	{
		DrawLine(glm::vec3(start, 0.0f), glm::vec3(end, 0.0f), color);
	}
	void Renderer2D::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.LineIndexCount >= s_Data.MaxLineVertices)
			NextBatchLines();

		s_Data.LineVertexBufferPtr->Position = start;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = end;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineIndexCount += 2;

		s_Data.Stats.LineCount++;
	}


	void Renderer2D::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommands::SetViewport(0, 0, width, height);
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	const Renderer2D::Statistics& Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(s_Data.Stats));
	}



	float Renderer2D::GetTextureIndex(const Ref<Texture2D>& texture)
	{
		if (!texture) return 0.0f; // white tex

		for (size_t i{}; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
				return static_cast<float>(i);
		}

		// First use
		return PushTexture(texture);
	}

	float Renderer2D::PushTexture(const Ref<Texture2D>& texture)
	{
		EIS_CORE_ASSERT(texture, "Invalid texture!");

		if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
			NextBatchQuads();

		const float textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;

		return textureIndex;
	}
}