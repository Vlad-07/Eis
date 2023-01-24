#pragma once

#include "Eis/Renderer/Objects/VertexArray.h"

namespace Eis
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererId;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vb) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& ib) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	};
}