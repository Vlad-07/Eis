#include "Eispch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <Eis/Debug/Assert.h>


namespace Eis
{
	static constexpr uint32_t c_MaxFramebufferSize{ 8192 }; // should querry gpu


	static bool IsDepthFormat(FramebufferTexFormat format)
	{
		switch (format)
		{
			case FramebufferTexFormat::DEPTH24STENCIL8:
				return true;
		}
		return false;
	}

	static GLenum AttachmentToGLFormat(FramebufferTexFormat format)
	{
		switch (format)
		{
			case FramebufferTexFormat::RGB8:
				return GL_RGB8;
			case FramebufferTexFormat::RGBA8:
				return GL_RGBA8;
			case Eis::FramebufferTexFormat::DEPTH24STENCIL8:
				return GL_DEPTH24_STENCIL8;
		}

		EIS_CORE_ASSERT(false, "Invalid attachment!");
		return 0;
	}

	static void TextureSetup(GLuint id, uint32_t samples, FramebufferTexSpec spec, uint32_t width, uint32_t height)
	{
		if (samples > 1)
		{
			glTextureStorage2DMultisample(id, samples, AttachmentToGLFormat(spec.Format), width, height, GL_FALSE);
		}
		else
		{
			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glTextureStorage2D(id, 1, AttachmentToGLFormat(spec.Format), width, height);
		}
	}



	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& fbSpec)
		: m_Spec{ fbSpec }
	{
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
			glCreateTextures(GL_TEXTURE_2D, static_cast<GLsizei>(m_ColorIds.size()), m_ColorIds.data());

			for (GLenum i{}; i < m_ColorAttachmentSpecs.size(); i++)
			{
				TextureSetup(m_ColorIds[i], m_Spec.Samples, m_ColorAttachmentSpecs[i], m_Spec.Width, m_Spec.Height);
				glNamedFramebufferTexture(m_RendererId, GL_COLOR_ATTACHMENT0 + i, m_ColorIds[i], 0);
			}
		}

		if (m_DepthAttachmentSpec.Format != FramebufferTexFormat::NONE)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthId);

			TextureSetup(m_DepthId, m_Spec.Samples, m_DepthAttachmentSpec, m_Spec.Width, m_Spec.Height);
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

	void OpenGLFramebuffer::Clear()
	{
		for (GLint i{}; i < m_ColorAttachmentSpecs.size(); i++)
		{
			switch (m_ColorAttachmentSpecs[i].Format)
			{
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

		if (width >= c_MaxFramebufferSize || height >= c_MaxFramebufferSize)
		{
			EIS_CORE_ERROR("Requested framebuffer size too large!");
			return;
		}

		m_Spec.Width  = width;
		m_Spec.Height = height;


		// maybe only resize attachments
		InvalidateAttachments();
	}
}