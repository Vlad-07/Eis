#pragma once

#include "Eis/Renderer/Buffer.h"

namespace Eis
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_RendererId;

	public:
		OpenGLVertexBuffer(float* verticies, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_RendererId;
		uint32_t m_Count;

	public:
		OpenGLIndexBuffer(uint32_t* indicies, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const
		{
			return m_Count;
		}
	};
}