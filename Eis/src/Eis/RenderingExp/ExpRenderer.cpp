#include "Eispch.h"
#include "ExpRenderer.h"

#include "Eis/Core/Application.h"

#include "Eis/Rendering/Renderer/RenderCommands.h"
#include "Eis/Rendering/Objects/Shader.h"
#include "Eis/Rendering/Objects/VertexArray.h"
#include "Eis/Rendering/Objects/Framebuffer.h"

#include "Eis/Assets/Importers/ShaderImporter.h"


#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <glad/glad.h>


namespace Eis
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		glm::vec2 Normal;
		float LightInfluence;
	};

	struct ExpRendererData
	{
		static constexpr uint32_t MaxQuads = 10'000;
		static constexpr uint32_t MaxQuadVertices = MaxQuads * 4;
		static constexpr uint32_t MaxQuadIndices = MaxQuads * 6;

		#ifndef EIS_PLATFORM_WEB // HACK: check max tex slots at runtime
		static constexpr uint8_t MaxTextureSlots = 32;
		#else
		static constexpr uint8_t MaxTextureSlots = 16;
		#endif

		// Geometry

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::mat4 QuadVertexPositions{};
		glm::mat4x2 QuadVertexTexCoords{};
		glm::mat4x3 QuadVertexNormals{};

		Ref<Texture2D> WhiteTexture;

		uint16_t TextureSlotIndex = 1; // 0 is WhiteTex
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		Ref<Framebuffer> GeometryFramebuffer;

		// Lighting

		glm::vec3 AmbientLight{};
		std::vector<ExpRenderer::Light> Lights;

		Ref<VertexArray> LightVA;
		Ref<VertexBuffer> LightVB;
		Ref<Shader> LightShader;
		Ref<Framebuffer> LightFramebuffer;

		// Composition

		Ref<Shader> CompositionShader;
		Ref<VertexArray> FullscreenVA;

		Ref<Framebuffer> ScreenFramebuffer;
	};
	static ExpRendererData s_Data;

	static float tempLi{ 1.0f };


	void ExpRenderer::Init()
	{
		RenderCommands::Init();

		// Init Shaders

		int samplers[s_Data.MaxTextureSlots];
		for (uint8_t i{}; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.QuadShader = ShaderImporter::LoadShader("resources/shaders/QuadLit.glsl");
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);


		s_Data.LightShader = ShaderImporter::LoadShader("resources/shaders/Light.glsl");
		s_Data.LightShader->Bind();
		s_Data.LightShader->SetInt("u_Normal", 0);
		s_Data.LightShader->SetFloat2("u_ScreenSize",
			{ Application::GetWindow().GetWidth(),
			  Application::GetWindow().GetHeight() });


		s_Data.CompositionShader = ShaderImporter::LoadShader("resources/shaders/Composition.glsl");
		s_Data.CompositionShader->Bind();
		s_Data.CompositionShader->SetInt("u_Normal", 0);
		s_Data.CompositionShader->SetInt("u_Albedo", 1);
		s_Data.CompositionShader->SetInt("u_Light", 2);
		s_Data.CompositionShader->SetInt("u_VolumetricLight", 3);



		// Init Quads

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout(s_Data.QuadShader->GetAttributeLayout());

		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		uint32_t* quadsIndices = new uint32_t[s_Data.MaxQuadIndices];
		for (uint32_t i = 0, off = 0; i < s_Data.MaxQuadIndices; i += 6)
		{
			quadsIndices[i + 0] = off + 0;
			quadsIndices[i + 1] = off + 1;
			quadsIndices[i + 2] = off + 2;

			quadsIndices[i + 3] = off + 2;
			quadsIndices[i + 4] = off + 3;
			quadsIndices[i + 5] = off + 0;

			off += 4;
		}
		Ref<IndexBuffer> quadsIB = IndexBuffer::Create(quadsIndices, s_Data.MaxQuadIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadsIB);
		s_Data.QuadVertexArray->Unbind();
		delete[] quadsIndices;

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxQuadVertices];

		// Init WhiteTexture

		const uint32_t whiteTexData = 0xffffffff;
		Buffer data{ static_cast<const void*>(&whiteTexData), sizeof(whiteTexData) };
		s_Data.WhiteTexture = Texture2D::Create(TextureSpec{}, data);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;


		// Init QuadVertex

		// Vertex order
		// 3    2
		// 
		// 
		// 0    1

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.QuadVertexTexCoords[0] = { 0.0f, 0.0f };
		s_Data.QuadVertexTexCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadVertexTexCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadVertexTexCoords[3] = { 0.0f, 1.0f };

		s_Data.QuadVertexNormals[0] = { -1.0f, -1.0f, 1.0f };
		s_Data.QuadVertexNormals[1] = {  1.0f, -1.0f, 1.0f };
		s_Data.QuadVertexNormals[2] = {  1.0f,  1.0f, 1.0f };
		s_Data.QuadVertexNormals[3] = { -1.0f,  1.0f, 1.0f };

		// Init framebuffers

		FramebufferSpec gFbSpec;
		gFbSpec.Width = Application::GetWindow().GetWidth();
		gFbSpec.Height = Application::GetWindow().GetHeight();
		gFbSpec.AttachmentsSpec = {
			{ FramebufferTexFormat::RGB8 },
			{ FramebufferTexFormat::RGBA8, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } }
		};
		s_Data.GeometryFramebuffer = Framebuffer::Create(gFbSpec);

		FramebufferSpec lightFbSpec;
		lightFbSpec.Width = Application::GetWindow().GetWidth();
		lightFbSpec.Height = Application::GetWindow().GetHeight();
		lightFbSpec.AttachmentsSpec = {
			{ FramebufferTexFormat::RGB8 },
			{ FramebufferTexFormat::RGB8 }
		};
		s_Data.LightFramebuffer = Framebuffer::Create(lightFbSpec);

		FramebufferSpec compositeFbSpec;
		compositeFbSpec.Width = Application::GetWindow().GetWidth();
		compositeFbSpec.Height = Application::GetWindow().GetHeight();
		compositeFbSpec.SwapChainTarget = true;
		compositeFbSpec.AttachmentsSpec = {
			{ FramebufferTexFormat::RGBA8 }
		};
		s_Data.ScreenFramebuffer = Framebuffer::Create(compositeFbSpec);


		uint32_t quadIndices[6]{ 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, 6);


		s_Data.LightVB = VertexBuffer::Create(sizeof(glm::vec2) * 4);
		s_Data.LightVB->SetLayout(s_Data.LightShader->GetAttributeLayout());

		s_Data.LightVA = VertexArray::Create();
		s_Data.LightVA->AddVertexBuffer(s_Data.LightVB);
		s_Data.LightVA->SetIndexBuffer(quadIB);


		s_Data.FullscreenVA = VertexArray::Create();
	}

	void ExpRenderer::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
		s_Data.Lights.clear();
	}


	/*void ExpRenderer::BeginScene(const OrthographicCamera& camera)
	{
		const auto& vp{ camera.GetViewProjectionMatrix() };
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_VP", vp);
		s_Data.LightShader->Bind();
		s_Data.LightShader->SetMat4("u_VP", vp);

		StartBatch();

		// Begin Geometry pass
		s_Data.GeometryFramebuffer->Bind();
		s_Data.GeometryFramebuffer->Clear();

		glDisable(GL_BLEND);
	}*/

	void ExpRenderer::EndScene()
	{
		Flush();

		// Light pass

		glEnable(GL_BLEND);

		glBlendFunc(GL_ONE, GL_ONE);

		s_Data.LightFramebuffer->Bind();
		s_Data.LightFramebuffer->Clear();

		s_Data.LightShader->Bind();
		s_Data.GeometryFramebuffer->BindTextures();
		for (const Light& light : s_Data.Lights)
		{
			s_Data.LightShader->SetFloat2("LightPos", light.Position);
			s_Data.LightShader->SetFloat3("LightColor", light.Color * light.Intensity);
			s_Data.LightShader->SetFloat("LightRange", light.Range);
			s_Data.LightShader->SetFloat("LightVolume", light.Volume);
			// TODO: instance lights
			const glm::vec2 vertices[4]{
				light.Position + glm::vec2{-light.Range,-light.Range },
				light.Position + glm::vec2{ light.Range,-light.Range },
				light.Position + glm::vec2{ light.Range, light.Range },
				light.Position + glm::vec2{-light.Range, light.Range }
			};
			s_Data.LightVB->SetData((void*)&vertices, sizeof(vertices));

			RenderCommands::DrawIndexed(s_Data.LightVA);
		}

		// Composite pass

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		s_Data.ScreenFramebuffer->Bind();
		s_Data.ScreenFramebuffer->Clear();

		s_Data.CompositionShader->Bind();
		s_Data.CompositionShader->SetFloat3("u_AmbientLight", s_Data.AmbientLight);
		// there must be a better way to bind textures from different framebuffers
		s_Data.LightFramebuffer->BindTextures(s_Data.GeometryFramebuffer->GetAttachmentsNumber());

		RenderCommands::DrawArrays(s_Data.FullscreenVA, 3);
	}



	void ExpRenderer::StartBatch()
	{
		StartBatchQuads();
	}

	void ExpRenderer::StartBatchQuads()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1; // maybe don't throw away last frame's textures?
	}


	void ExpRenderer::Flush()
	{
		FlushQuads();
	}

	void ExpRenderer::FlushQuads()
	{
		if (s_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i{}; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		s_Data.QuadShader->Bind();
		RenderCommands::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}


	void ExpRenderer::NextBatchQuads()
	{
		FlushQuads();
		StartBatchQuads();
	}




	void ExpRenderer::DrawQuad(const QuadDesc& desc)
	{
		DrawRotatedQuad(desc.Pos, desc.Size, desc.Rotation, desc.Tex, desc.Tiling, desc.Tint);
	}

	void ExpRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, color);
	}
	void ExpRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, nullptr, 1.0f, color);
	}
	void ExpRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, texture, tiling, tint);
	}
	void ExpRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		DrawRotatedQuad(position, size, 0.0f, texture, tiling, tint);
	}

	void ExpRenderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color);
	}
	void ExpRenderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(position, size, rotation, nullptr, 1.0f, color);
	}
	void ExpRenderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, texture, tiling, tint);
	}
	void ExpRenderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const glm::mat4 transformed = glm::translate(glm::mat4{ 1.0f }, position)
			* (rotation != 0.0f ? glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotation), glm::vec3{ 0.0f, 0.0f, 1.0f }) : 1.0f)
			* glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x, size.y, 1.0f })
			* s_Data.QuadVertexPositions;

		DrawQuad(glm::mat4x3{ transformed }, texture, tiling, tint);
	}

	void ExpRenderer::DrawQuad(const glm::mat4x3& worldVerts, const glm::vec4& color)
	{
		DrawQuad(worldVerts, nullptr, 1.0f, color);
	}
	void ExpRenderer::DrawQuad(const glm::mat4x3& worldVerts, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxQuadIndices)
			NextBatchQuads();

		const float textureIndex = GetTextureIndex(texture);
		for (uint8_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = worldVerts[i];
			s_Data.QuadVertexBufferPtr->Color = tint;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadVertexTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tiling;
			s_Data.QuadVertexBufferPtr->Normal = s_Data.QuadVertexNormals[i]; //* 0.0f */ + glm::vec2{0.0f, 1.0f};
			s_Data.QuadVertexBufferPtr->LightInfluence = tempLi;

			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

	//	NextBatchQuads();
	}

	void ExpRenderer::TmpLi(float infl)
	{
		tempLi = infl;
	}

	void ExpRenderer::SetAmbientLight(glm::vec3 col, float intensity)
	{
		s_Data.AmbientLight = col * intensity;
	}

	void ExpRenderer::AddLight(const Light& light)
	{
		s_Data.Lights.push_back(light);
	}


	float ExpRenderer::GetTextureIndex(const Ref<Texture2D>& texture)
	{
		if (!texture) return 0.0f; // white tex

		for (size_t i{}; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
				return static_cast<float>(i);
		}

		return PushTexture(texture);
	}

	float ExpRenderer::PushTexture(const Ref<Texture2D>& texture)
	{
		EIS_CORE_ASSERT(texture, "Invalid texture!");

		if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
			NextBatchQuads();

		const float textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;

		return textureIndex;
	}

	void ExpRenderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		s_Data.GeometryFramebuffer->Resize(width, height);
		s_Data.LightFramebuffer->Resize(width, height);
		s_Data.ScreenFramebuffer->Resize(width, height);

		s_Data.LightShader->Bind();
		s_Data.LightShader->SetFloat2("u_ScreenSize", glm::vec2{ width, height });
	}
}