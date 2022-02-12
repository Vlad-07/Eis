#pragma once

#include "Eis/Renderer/Objects/VertexArray.h"

namespace Eis
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t m_RendererId;
		uint32_t m_VertexBufferIndexOffset = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	};
}