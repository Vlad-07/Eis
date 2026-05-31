#include "Eispch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>


namespace Eis
{
	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Bool:		return GL_BOOL;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Float:		return GL_FLOAT;
		}

		EIS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glGenVertexArrays(1, &m_RendererId);
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

		EIS_CORE_ASSERT(vb->GetLayout().GetAttributes().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererId);
		vb->Bind();

		const auto& vbLayout = vb->GetLayout();

		for (const auto& element : vbLayout)
		{
			switch (element.DataType)
			{
				case ShaderDataType::Bool:
				case ShaderDataType::Int:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(m_VertexBufferIndex,
						element.ComponentCount,
						ShaderDataTypeToOpenGLType(element.DataType),
						vbLayout.GetStride(),
						(const void*)element.Offset);

					m_VertexBufferIndex++;

					break;
				}

				case ShaderDataType::Float:
				{
					if (element.Colums == 1)
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(m_VertexBufferIndex,
							element.ComponentCount,
							ShaderDataTypeToOpenGLType(element.DataType),
							element.Normalized ? GL_TRUE : GL_FALSE,
							vbLayout.GetStride(),
							(const void*)element.Offset);

						m_VertexBufferIndex++;
					}
					else
					{
						for (size_t i{}; i < element.ComponentCount; i++)
						{
							glEnableVertexAttribArray(m_VertexBufferIndex);
							glVertexAttribPointer(m_VertexBufferIndex,
								element.ComponentCount,
								ShaderDataTypeToOpenGLType(element.DataType),
								element.Normalized ? GL_TRUE : GL_FALSE,
								vbLayout.GetStride(),
								(const void*)(element.Offset + sizeof(float) * element.ComponentCount * i));
							glVertexAttribDivisor(m_VertexBufferIndex, 1);
							m_VertexBufferIndex++;
						}
					}
					break;
				}

				default:
					EIS_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
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