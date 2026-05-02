#include "Eispch.h"
#include "WebGLVertexArray.h"

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

		EIS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	void WebGLVertexArray::Bind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		GLuint vbIndex = 0;
		for (const auto& vb : m_VertexBuffers)
		{
			vb->Bind();

			const auto& vbLayout = vb->GetLayout();
			for (const auto& element : vbLayout)
			{
				switch (element.Type)
				{
					case ShaderDataType::Bool:
					case ShaderDataType::Int:
					case ShaderDataType::Int2:
					case ShaderDataType::Int3:
					case ShaderDataType::Int4:
					case ShaderDataType::Float:
					case ShaderDataType::Float2:
					case ShaderDataType::Float3:
					case ShaderDataType::Float4:
					{
						glEnableVertexAttribArray(vbIndex);
						glVertexAttribPointer(vbIndex,
							element.GetComponentCount(),
							ShaderDataTypeToOpenGLType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							vbLayout.GetStride(),
							(const void*)element.Offset);

						vbIndex++;

						break;
					}

					case ShaderDataType::Mat3:
					case ShaderDataType::Mat4:
					{
						uint8_t count = element.GetComponentCount();
						for (uint8_t i{}; i < count; i++)
						{
							glEnableVertexAttribArray(vbIndex);
							glVertexAttribPointer(vbIndex,
								count,
								ShaderDataTypeToOpenGLType(element.Type),
								element.Normalized ? GL_TRUE : GL_FALSE,
								vbLayout.GetStride(),
								(const void*)(element.Offset + sizeof(float) * count * i));
							glVertexAttribDivisor(vbIndex, 1);
							vbIndex++;
						}

						break;
					}

					default:
						EIS_CORE_ASSERT(false, "Unknown ShaderDataType!");
				}
			}
		}

		if (m_IndexBuffer)
			m_IndexBuffer->Bind();
	}

	void WebGLVertexArray::Unbind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void WebGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		EIS_CORE_ASSERT(vb->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		m_VertexBuffers.push_back(vb);
	}

	void WebGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_IndexBuffer = ib;
	}
}