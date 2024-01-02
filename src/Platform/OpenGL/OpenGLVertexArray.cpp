#include "Eispch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Eis
{
	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		}

		EIS_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glCreateVertexArrays(1, &m_RendererId);
		// glGenVertexArrays(1, &m_RendererId);  // Use if OpenGL version is smaller than 4.5
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		EIS_CORE_ASSERT(vb->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererId);
		vb->Bind();

		const auto& vbLayout = vb->GetLayout();

		for (const auto& element : vbLayout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				vbLayout.GetStride(),
				(const void*)element.Offset);

			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vb);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(m_RendererId);
		ib->Bind();

		m_IndexBuffer = ib;
	}
}