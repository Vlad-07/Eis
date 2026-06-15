#include "Eispch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>


namespace Eis
{
	static GLenum ShaderDataTypeToOpenGLType(BaseDataType type)
	{
		switch (type)
		{
			case BaseDataType::Bool:  return GL_BOOL;
			case BaseDataType::Int:   return GL_INT;
			case BaseDataType::Float: return GL_FLOAT;
		}

		EIS_CORE_ASSERT(false);
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

		EIS_CORE_ASSERT(vb->GetLayout().Attributes.size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererId);
		vb->Bind();

		const auto& vbLayout = vb->GetLayout();

		for (const auto& element : vbLayout.Attributes)
		{
			switch (element.DataType)
			{
				case BaseDataType::Bool:
				case BaseDataType::Int:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(m_VertexBufferIndex,
						element.VecSize,
						ShaderDataTypeToOpenGLType(element.DataType),
						vbLayout.Stride,
						(const void*)element.ByteOffset);

					m_VertexBufferIndex++;

					break;
				}

				case BaseDataType::Float:
				{
					if (element.Columns == 1)
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(m_VertexBufferIndex,
							element.VecSize,
							ShaderDataTypeToOpenGLType(element.DataType),
							element.Normalized ? GL_TRUE : GL_FALSE,
							vbLayout.Stride,
							(const void*)element.ByteOffset);

						m_VertexBufferIndex++;
					}
					else
					{
						for (size_t i{}; i < element.VecSize; i++)
						{
							glEnableVertexAttribArray(m_VertexBufferIndex);
							glVertexAttribPointer(m_VertexBufferIndex,
								element.VecSize,
								ShaderDataTypeToOpenGLType(element.DataType),
								element.Normalized ? GL_TRUE : GL_FALSE,
								vbLayout.Stride,
								(const void*)(element.ByteOffset + sizeof(float) * element.VecSize * i));
							glVertexAttribDivisor(m_VertexBufferIndex, 1);
							m_VertexBufferIndex++;
						}
					}
					break;
				}

				default:
					EIS_CORE_ASSERT(false);
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