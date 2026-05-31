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
			case ShaderDataType::Float:		return GL_FLOAT;
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
				switch (element.DataType)
				{
					case ShaderDataType::Bool:
					case ShaderDataType::Int:
					{
						glEnableVertexAttribArray(vbIndex);
						glVertexAttribIPointer(vbIndex,
							element.ComponentCount,
							ShaderDataTypeToOpenGLType(element.DataType),
							vbLayout.GetStride(),
							(const void*)element.Offset);

						vbIndex++;

						break;
					}

					case ShaderDataType::Float:
					{
						if (element.Colums == 1)
						{
							glEnableVertexAttribArray(vbIndex);
							glVertexAttribPointer(vbIndex,
								element.ComponentCount,
								ShaderDataTypeToOpenGLType(element.DataType),
								element.Normalized ? GL_TRUE : GL_FALSE,
								vbLayout.GetStride(),
								(const void*)element.Offset);

							vbIndex++;
						}
						else
						{
							for (size_t i{}; i < element.ComponentCount; i++)
							{
								glEnableVertexAttribArray(vbIndex);
								glVertexAttribPointer(vbIndex,
									element.ComponentCount,
									ShaderDataTypeToOpenGLType(element.DataType),
									element.Normalized ? GL_TRUE : GL_FALSE,
									vbLayout.GetStride(),
									(const void*)(element.Offset + sizeof(float) * element.ComponentCount * i));
								glVertexAttribDivisor(vbIndex, 1);
								vbIndex++;
							}
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

		EIS_CORE_ASSERT(vb->GetLayout().GetAttributes().size(), "Vertex Buffer has no layout!");
		
		m_VertexBuffers.push_back(vb);
	}

	void WebGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_IndexBuffer = ib;
	}
}