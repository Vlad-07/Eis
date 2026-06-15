#include "Eispch.h"
#include "WebGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>


namespace Eis
{
	static GLenum ImageToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::RGB8: return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
		}

		EIS_CORE_ASSERT(false);
		return 0;
	}

	static GLenum ImageToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::RGB8: return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
		}

		EIS_CORE_ASSERT(false);
		return 0;
	}


	WebGLTexture2D::WebGLTexture2D(const TextureSpec& spec, Buffer data)
		: m_Spec{ spec }
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_DataFormat = ImageToGLDataFormat(m_Spec.Format);
		m_InternalFormat = ImageToGLInternalFormat(m_Spec.Format);

		glGenTextures(1, &m_RendererId);
		glBindTexture(GL_TEXTURE_2D, m_RendererId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Spec.Width, m_Spec.Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);

		if (data) SetData(data);
	}

	WebGLTexture2D::~WebGLTexture2D()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteTextures(1, &m_RendererId);
	}


	void WebGLTexture2D::SetData(Buffer data)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		EIS_CORE_ASSERT(data.Size == m_Spec.Width * m_Spec.Height * bpp, "Data must cover entire texture!");

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Spec.Width, m_Spec.Height, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
	}

	void WebGLTexture2D::Bind(uint32_t slot) const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererId);
	}
}