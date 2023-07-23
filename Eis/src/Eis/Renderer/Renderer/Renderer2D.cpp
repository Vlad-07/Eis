#include "Eispch.h"

#include "Eis/Renderer/Renderer/Renderer2D.h"

#include "Eis/Renderer/Objects/VertexArray.h"
#include "Eis/Renderer/Objects/Shader.h"
#include "Eis/Renderer/Renderer/RenderCommands.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Eis
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Shader> CircleShader;
		Ref<Shader> LineShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Scope<Renderer2DStorage> s_Data;

	void Renderer2D::Init()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data = CreateScope<Renderer2DStorage>();

		// Init QuadVertexArray
		float quadVerts[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		s_Data->QuadVertexArray = VertexArray::Create();
		Ref<VertexBuffer> VB;
		VB = VertexBuffer::Create(quadVerts, sizeof(quadVerts));

		VB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data->QuadVertexArray->AddVertexBuffer(VB);

		uint32_t quadIndices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> IB = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(IB);


		// Init WhiteTexture
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTexData, sizeof(whiteTexData));

		// Init Shaders
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->CircleShader = Shader::Create("assets/shaders/Circle.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->LineShader = Shader::Create("assets/shaders/Line.glsl");
		s_Data->LineShader->Bind();
		s_Data->LineShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->CircleShader->Bind();
		s_Data->CircleShader->SetMat4("u_VP", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_VP", camera.GetViewProjectionMatrix());

		s_Data->LineShader->Bind();
		s_Data->LineShader->SetMat4("u_VP", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

	}



	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_Tiling", 1.0f);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		DrawQuad(glm::vec3(position, 0.0f), size, texture, tiling, tint);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tint)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", tint);
		s_Data->TextureShader->SetFloat("u_Tiling", tiling);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_Tiling", 1.0f);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		DrawRotatedQuad(glm::vec3(position, 1.0f), size, rotation, texture, tint);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", tint);
		s_Data->TextureShader->SetFloat("u_Tiling", 1.0f);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}



	void Renderer2D::DrawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawCircle(glm::vec3(position, 0.0f), size, color);
	}
	void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->CircleShader->Bind();
		s_Data->CircleShader->SetFloat4("u_Color", color);
		s_Data->CircleShader->SetFloat("u_Thickness", 1.0f); // TODO: access these parameters
		s_Data->CircleShader->SetFloat("u_Fade", 0.005f);

		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) /*  *rotation */ * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->CircleShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawCircle(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		DrawCircle(glm::vec3(position, 0.0f), size, texture, tint);
	}
	void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->CircleShader->Bind();
		s_Data->CircleShader->SetFloat4("u_Color", tint);
		s_Data->CircleShader->SetFloat("u_Thickness", 1.0f);
		s_Data->CircleShader->SetFloat("u_Fade", 0.005f);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) /*  *rotation */ * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->CircleShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}



	void Renderer2D::DrawLine(const glm::vec3& start, const float& angle, const float& length, const glm::vec4& color, const float& width)
	{
		glm::vec3 offset = glm::rotate(glm::vec3(0.0f, length, 0.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 end = start + offset;
		DrawLine(start, end, color, width);
	}
	void Renderer2D::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, const float& width) // hacked
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		s_Data->LineShader->Bind();
		s_Data->LineShader->SetFloat4("u_Color", color);
		s_Data->LineShader->SetMat4("u_Transform", glm::mat4(1.0f));

		float verts[] = { start.x, start.y, start.z, end.x, end.y, end.z };
		Ref<VertexArray> lineVertexArray = VertexArray::Create();
		Ref<VertexBuffer> vb = VertexBuffer::Create(verts, sizeof(verts));
		vb->SetLayout({
				{ ShaderDataType::Float3, "a_Position" }
			});
		lineVertexArray->AddVertexBuffer(vb);

		uint32_t indices[3 * 2] = { 0, 1 };
		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		lineVertexArray->SetIndexBuffer(ib);

		RenderCommands::DrawLines(lineVertexArray, width);
	}
}