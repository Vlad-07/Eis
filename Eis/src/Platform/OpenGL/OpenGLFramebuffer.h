#pragma once

#include "Eis/Rendering/Objects/Framebuffer.h"


typedef unsigned int GLuint;

namespace Eis
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& fbSpec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Clear() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void BindTextures(uint32_t offset = 0) override;
		virtual void BindDepthTexture(uint32_t binding = 0) override;
		virtual uint32_t GetAttachmentsNumber() const override { return static_cast<uint32_t>(m_ColorIds.size()); }

		virtual void Blit(const Ref<Framebuffer>& target, AttachmentBit bit, uint32_t srcAttachment = 0, uint32_t dstAttachment = 0) const override;

		// Only for R32I!
		virtual int ReadPixel(uint32_t attachmentId, int x, int y) override;

		virtual const std::vector<GLuint>& GetColorAttachmentsIds() const override { return m_ColorIds; }

		virtual const FramebufferSpec& GetSpec() const override { return m_Spec; }

	private:
		void InvalidateAttachments();

	private:
		FramebufferSpec m_Spec;

		GLuint m_RendererId{};

		std::vector<FramebufferTexSpec> m_ColorAttachmentSpecs;
		FramebufferTexSpec m_DepthAttachmentSpec{ FramebufferTexFormat::NONE };

		std::vector<GLuint> m_ColorIds;
		GLuint m_DepthId{};
	};
}