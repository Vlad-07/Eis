#include "Eispch.h"
#include "WebGLVertexArray.h"

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

		EIS_CORE_ASSERT(false, "Unknown BaseDataType!");
		return 0;
	}


	void WebGLVertexArray::Bind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		GLuint vbIndex{};
		for (const auto& vb : m_VertexBuffers)
		{
			vb->Bind();

			const auto& vbLayout = vb->GetLayout();
			for (const auto& element : vbLayout.Attributes)
			{
				switch (element.DataType)
				{
					case BaseDataType::Bool:
					case BaseDataType::Int:
					{
						glEnableVertexAttribArray(vbIndex);
						glVertexAttribIPointer(vbIndex,
							element.VecSize,
							ShaderDataTypeToOpenGLType(element.DataType),
							vbLayout.Stride,
							(const void*)element.ByteOffset);

						vbIndex++;

						break;
					}

					case BaseDataType::Float:
					{
						if (element.Columns == 1)
						{
							glEnableVertexAttribArray(vbIndex);
							glVertexAttribPointer(vbIndex,
								element.VecSize,
								ShaderDataTypeToOpenGLType(element.DataType),
								element.Normalized ? GL_TRUE : GL_FALSE,
								vbLayout.Stride,
								(const void*)element.ByteOffset);

							vbIndex++;
						}
						else
						{
							for (size_t i{}; i < element.VecSize; i++)
							{
								glEnableVertexAttribArray(vbIndex);
								glVertexAttribPointer(vbIndex,
									element.VecSize,
									ShaderDataTypeToOpenGLType(element.DataType),
									element.Normalized ? GL_TRUE : GL_FALSE,
									vbLayout.Stride,
									(const void*)(element.ByteOffset + sizeof(float) * element.VecSize * i));
								glVertexAttribDivisor(vbIndex, 1);
								vbIndex++;
							}
						}
						break;
					}

					default:
						EIS_CORE_ASSERT(false, "Unknown BaseDataType!");
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

		EIS_CORE_ASSERT(vb->GetLayout().Attributes.size(), "Vertex Buffer has no layout!");
		
		m_VertexBuffers.push_back(vb);
	}

	void WebGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_IndexBuffer = ib;
	}
}