#include "Eispch.h"
#include "Renderer2D.h"

#include "Eis/Renderer/Objects/VertexArray.h"
#include "Eis/Renderer/Objects/Shader.h"
#include "Eis/Renderer/Renderer/RenderCommands.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eis
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Scope<Renderer2DStorage> s_Data;

	void Renderer2D::Init()
	{
		s_Data = CreateScope<Renderer2DStorage>();

		float verts2[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		s_Data->QuadVertexArray = VertexArray::Create();
		Ref<VertexBuffer> VB;
		VB = VertexBuffer::Create(verts2, sizeof(verts2));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		VB->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(VB);

		uint32_t indices[3 * 2] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> IB;
		IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(IB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTexData, sizeof(whiteTexData));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		s_Data.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_VP", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) /*  *rotation */ * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) /*  *rotation */ * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();

		RenderCommands::DrawIndexed(s_Data->QuadVertexArray);
	}
}