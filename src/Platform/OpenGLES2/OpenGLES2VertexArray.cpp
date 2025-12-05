#include "Eispch.h"
#include "OpenGLES2VertexArray.h"

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


	OpenGLES2VertexArray::OpenGLES2VertexArray()
	{
		EIS_PROFILE_RENDERER_FUNCTION();
		
		m_RendererId = 0;
	}

	OpenGLES2VertexArray::~OpenGLES2VertexArray()
	{
		EIS_PROFILE_RENDERER_FUNCTION();
	}

	void OpenGLES2VertexArray::Bind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		uint8_t vbIndex = 0;
		for (const auto& vb : m_VertexBuffers)
		{
			vb->Bind();

			const auto& vbLayout = vb->GetLayout();
			for (const auto& element : vbLayout)
			{
				glEnableVertexAttribArray(vbIndex);
				glVertexAttribPointer(vbIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					vbLayout.GetStride(),
					(const void*)element.Offset);
				vbIndex++;
			}
		}

		m_IndexBuffer->Bind();
	}

	void OpenGLES2VertexArray::Unbind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLES2VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		EIS_CORE_ASSERT(vb->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		m_VertexBuffers.push_back(vb);
	}

	void OpenGLES2VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_IndexBuffer = ib;
	}
}