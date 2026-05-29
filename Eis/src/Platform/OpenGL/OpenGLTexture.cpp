#include "Eispch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>


namespace Eis
{
	static GLenum ImageToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
			case Eis::ImageFormat::RGB8: return GL_RGB;
			case Eis::ImageFormat::RGBA8: return GL_RGBA;
		}

		EIS_CORE_ASSERT(false);
		return 0;
	}

	static GLenum ImageToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
			case Eis::ImageFormat::RGB8: return GL_RGB8;
			case Eis::ImageFormat::RGBA8: return GL_RGBA8;
		}

		EIS_CORE_ASSERT(false);
		return 0;
	}


	OpenGLTexture2D::OpenGLTexture2D(const TextureSpec& spec, Buffer data)
		: m_Spec{ spec }
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_DataFormat = ImageToGLDataFormat(m_Spec.Format);
		m_InternalFormat = ImageToGLInternalFormat(m_Spec.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Spec.Width, m_Spec.Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (data) SetData(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteTextures(1, &m_RendererId);
	}


	void OpenGLTexture2D::SetData(Buffer data)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		const uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		EIS_CORE_ASSERT(data.Size == m_Spec.Width * m_Spec.Height * bpp, "Data must cover entire texture!");

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Spec.Width, m_Spec.Height, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindTextureUnit(slot, m_RendererId);
	}
}