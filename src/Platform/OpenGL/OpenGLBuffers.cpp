#include "Eispch.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>


namespace Eis
{
	// Vertex Buffer -----------------------------------------------------------------------------------------------|
	// -------------------------------------------------------------------------------------------------------------|

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glCreateBuffers(1, &m_RendererId);
		// glGenBuffers(1, &m_RendererId);  // Use if OpenGL version is smaller than 4.5
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


	// Index Buffer ------------------------------------------------------------------------------------------------|
	// -------------------------------------------------------------------------------------------------------------|

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indicies, uint32_t count)
		: m_Count(count)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glCreateBuffers(1, &m_RendererId);
		// glGenBuffers(1, &m_RendererId);  // Use if OpenGL version is smaller than 4.5
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, GL_STATIC_DRAW);
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
}