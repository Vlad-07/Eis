#pragma once

#include "Eis/Rendering/Objects/Buffers.h"

typedef unsigned int GLuint;

// TODO: centralize RendererAPI types
namespace Eis
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const void* verticies, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const AttributeLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const AttributeLayout& layout) override { m_Layout = layout; }

	private:
		GLuint m_RendererId{};
		AttributeLayout m_Layout;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		GLuint m_RendererId{};
		uint32_t m_Count{};
	};


	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint64_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint64_t size, uint64_t offset = 0) override;

	private:
		GLuint m_RendererId{};
		uint64_t m_Size{};
	};
}