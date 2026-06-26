#include "Eispch.h"
#include "OpenGLFramebuffer.h"

#include "Eis/Debug/Assert.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Eis
{
	namespace
	{
		constexpr uint32_t c_MaxFramebufferSize{ 8192 }; // TODO: querry gpu

		bool IsInteger(FramebufferTexFormat format)
		{
			if (format == FramebufferTexFormat::R32I)
				return true;
			return false;
		}

		bool IsDepthFormat(FramebufferTexFormat format)
		{
			switch (format)
			{
				case FramebufferTexFormat::DEPTH16:
				case FramebufferTexFormat::DEPTH24:
				case FramebufferTexFormat::DEPTH24STENCIL8:
					return true;
			}
			return false;
		}

		GLenum FormatToGLFormat(FramebufferTexFormat format)
		{
			switch (format)
			{
				case FramebufferTexFormat::R32I:
					return GL_R32I;
				case FramebufferTexFormat::RGB8:
					return GL_RGB8;
				case FramebufferTexFormat::RGBA8:
					return GL_RGBA8;
				case FramebufferTexFormat::DEPTH16:
					return GL_DEPTH_COMPONENT16;
				case FramebufferTexFormat::DEPTH24:
					return GL_DEPTH_COMPONENT24;
				case FramebufferTexFormat::DEPTH24STENCIL8:
					return GL_DEPTH24_STENCIL8;
			}
			EIS_CORE_ASSERT(false, "Invalid attachment!");
			return 0;
		}

		GLenum DepthFormatToGLAttachmentType(FramebufferTexFormat format)
		{
			switch (format)
			{
				case FramebufferTexFormat::DEPTH16:
				case FramebufferTexFormat::DEPTH24:
					return GL_DEPTH_ATTACHMENT;

				case FramebufferTexFormat::DEPTH24STENCIL8:
					return GL_DEPTH_STENCIL_ATTACHMENT;
			}
			EIS_CORE_ASSERT(false);
			return 0;
		}

		void CreateTextures(GLuint* ids, GLsizei count, uint32_t samples)
		{
			if (samples == 1)
				glCreateTextures(GL_TEXTURE_2D, count, ids);
			else
				glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, count, ids);
		}

		void TextureSetup(GLuint id, const FramebufferTexSpec& spec, uint32_t width, uint32_t height, uint32_t samples)
		{
			if (samples == 1)
			{
				glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTextureStorage2D(id, 1, FormatToGLFormat(spec.Format), width, height);
			}
			else
			{
				glTextureStorage2DMultisample(id, samples, FormatToGLFormat(spec.Format), width, height, GL_FALSE);
			}
		}
	}



	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& fbSpec)
		: m_Spec{ fbSpec }
	{
		EIS_CORE_ASSERT(fbSpec.Samples);

		for (const auto& spec : m_Spec.AttachmentsSpec.Attachments)
		{
			if (!IsDepthFormat(spec.Format))
				m_ColorAttachmentSpecs.push_back(spec);
			else if (m_DepthAttachmentSpec.Format == FramebufferTexFormat::NONE)
				m_DepthAttachmentSpec = spec;
			else
				EIS_CORE_ASSERT(false, "Only one depth buffer can be specified!");
		}

		if (!m_Spec.SwapChainTarget)
			glCreateFramebuffers(1, &m_RendererId);

		InvalidateAttachments();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteTextures(static_cast<GLsizei>(m_ColorIds.size()), m_ColorIds.data());
		glDeleteTextures(1, &m_DepthId);
		glDeleteFramebuffers(1, &m_RendererId);
	}

	void OpenGLFramebuffer::InvalidateAttachments()
	{
		glDeleteTextures(static_cast<GLsizei>(m_ColorIds.size()), m_ColorIds.data());
		glDeleteTextures(1, &m_DepthId);
		m_ColorIds.clear();
		m_DepthId = 0;

		// Attachments
		m_ColorIds.resize(m_ColorAttachmentSpecs.size());
		if (!m_ColorAttachmentSpecs.empty())
		{
			CreateTextures(m_ColorIds.data(), static_cast<GLsizei>(m_ColorIds.size()), m_Spec.Samples);

			for (GLenum i{}; i < m_ColorAttachmentSpecs.size(); i++)
			{
				TextureSetup(m_ColorIds[i], m_ColorAttachmentSpecs[i], m_Spec.Width, m_Spec.Height, m_Spec.Samples);
				glNamedFramebufferTexture(m_RendererId, GL_COLOR_ATTACHMENT0 + i, m_ColorIds[i], 0);
			}
		}

		if (m_DepthAttachmentSpec.Format != FramebufferTexFormat::NONE)
		{
			CreateTextures(&m_DepthId, 1, m_Spec.Samples);
			TextureSetup(m_DepthId, m_DepthAttachmentSpec, m_Spec.Width, m_Spec.Height, m_Spec.Samples);

			glNamedFramebufferTexture(m_RendererId, DepthFormatToGLAttachmentType(m_DepthAttachmentSpec.Format), m_DepthId, 0);
		}

		// Draw buffers
		if (m_ColorAttachmentSpecs.size() > 1)
		{
			EIS_CORE_ASSERT(m_ColorAttachmentSpecs.size() < 4, "Too many color attachments!");
			// TODO: check GL_MAX_COLOR_ATTACHMENTS

			const GLenum buffers[4]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glNamedFramebufferDrawBuffers(m_RendererId, static_cast<GLsizei>(m_ColorAttachmentSpecs.size()), buffers);
		}
		else if (m_ColorAttachmentSpecs.empty())
		{
			glNamedFramebufferDrawBuffer(m_RendererId, GL_NONE);
		}


		EIS_CORE_ASSERT(glCheckNamedFramebufferStatus(m_RendererId, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer incomplete!");
	}



	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BindTextures(uint32_t offset)
	{
		// TODO: check max texture slots
		EIS_CORE_ASSERT(offset + m_ColorIds.size() < 32, "Not enough texture slots available!");
		for (GLuint i{}; i < m_ColorIds.size(); i++)
		{
			glBindTextureUnit(i + offset, m_ColorIds[i]);
		}
	}

	void OpenGLFramebuffer::BindDepthTexture(uint32_t binding)
	{
		glBindTextureUnit(binding, m_DepthId);
	}

	void OpenGLFramebuffer::Clear()
	{
		for (GLint i{}; i < m_ColorAttachmentSpecs.size(); i++)
		{
			switch (m_ColorAttachmentSpecs[i].Format)
			{
				case FramebufferTexFormat::R32I:
				{
					GLint clearVal = static_cast<GLint>(std::get<int>(m_ColorAttachmentSpecs[i].ClearValue));
					glClearNamedFramebufferiv(m_RendererId, GL_COLOR, i, &clearVal);
					break;
				}

				case FramebufferTexFormat::RGB8:
				case FramebufferTexFormat::RGBA8:
				{
					const auto& clearColor = std::get<glm::vec4>(m_ColorAttachmentSpecs[i].ClearValue);
					glClearNamedFramebufferfv(m_RendererId, GL_COLOR, i, glm::value_ptr(clearColor));
					break;
				}
			}
		}

		if (m_DepthAttachmentSpec.Format != FramebufferTexFormat::NONE)
		{
			const auto& clearVals = std::get<DepthStencilClear>(m_DepthAttachmentSpec.ClearValue);
			switch (m_DepthAttachmentSpec.Format)
			{
				case FramebufferTexFormat::DEPTH16:
				case FramebufferTexFormat::DEPTH24:
					glClearNamedFramebufferfv(m_RendererId, GL_DEPTH, 0, &clearVals.Depth);
					break;

				case FramebufferTexFormat::DEPTH24STENCIL8:
					glClearNamedFramebufferfi(m_RendererId, GL_DEPTH_STENCIL, 0, clearVals.Depth, clearVals.Stencil);
					break;
			}
		}
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == m_Spec.Width && height == m_Spec.Height)
			return;

		if (width == 0 || height == 0
			|| width >= c_MaxFramebufferSize || height >= c_MaxFramebufferSize)
		{
			EIS_CORE_ERROR("Requested invalid framebuffer size: {}, {}", width, height);
			return;
		}

		m_Spec.Width  = width;
		m_Spec.Height = height;

		InvalidateAttachments();
	}


	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentId, int x, int y)
	{
		EIS_CORE_ASSERT(attachmentId < m_ColorIds.size(), "Invald attachment id!");

		if (x < 0 || y < 0
			|| static_cast<uint32_t>(x) >= m_Spec.Width
			|| static_cast<uint32_t>(y) >= m_Spec.Height)
		{
			EIS_CORE_WARN("Invalid pixel requested: {}, {}!", x, y);
			return 0;
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererId);
		glNamedFramebufferReadBuffer(m_RendererId, GL_COLOR_ATTACHMENT0 + attachmentId);

		int pixelData{};
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, static_cast<void*>(&pixelData));
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		return pixelData;
	}

	void OpenGLFramebuffer::Blit(const Ref<Framebuffer>& target, AttachmentBit bit, uint32_t srcAttachment, uint32_t dstAttachment) const
	{
		const auto& fb = *dynamic_cast<const OpenGLFramebuffer*>(target.get());

		GLbitfield bits{};
		GLenum filter{};
		switch (bit)
		{
			case Eis::AttachmentBit::Color:
				glNamedFramebufferReadBuffer(m_RendererId, GL_COLOR_ATTACHMENT0 + srcAttachment);
				glNamedFramebufferDrawBuffer(fb.m_RendererId, GL_COLOR_ATTACHMENT0 + dstAttachment);
				bits = GL_COLOR_BUFFER_BIT;
				filter = IsInteger(m_Spec.AttachmentsSpec.Attachments[srcAttachment].Format) ? GL_NEAREST : GL_LINEAR;
				break;

			case Eis::AttachmentBit::Depth:
				bits = GL_DEPTH_BUFFER_BIT;
				filter = GL_NEAREST;
				break;

			case Eis::AttachmentBit::Stencil:
				bits = GL_STENCIL_BUFFER_BIT;
				filter = GL_NEAREST;
				break;

			case Eis::AttachmentBit::DepthStencil:
				bits = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				filter = GL_NEAREST;
				break;

			default: EIS_CORE_ASSERT(false);
		}

		glBlitNamedFramebuffer(m_RendererId, fb.m_RendererId,
			0, 0, m_Spec.Width, m_Spec.Height,
			0, 0, fb.m_Spec.Width, fb.m_Spec.Height,
			bits, filter);
	}
}