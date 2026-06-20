#include "Eispch.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>


namespace Eis
{
	// Vertex Buffer

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* verticies, uint32_t size)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}



	// Index Buffer

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}



	// Uniform Buffer

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