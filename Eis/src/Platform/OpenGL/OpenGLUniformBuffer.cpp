#include "Eispch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>


namespace Eis
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint64_t size, uint32_t binding)
		: m_Size{ size }
	{
		glCreateBuffers(1, &m_RendererId);
		glNamedBufferData(m_RendererId, size, nullptr, GL_DYNAMIC_DRAW); // TODO: usage?
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererId);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint64_t size, uint64_t offset)
	{
		EIS_CORE_ASSERT(size + offset <= m_Size, "Data too large!");

		glNamedBufferSubData(m_RendererId, offset, size, data);
	}
}