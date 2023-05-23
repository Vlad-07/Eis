#include "Eispch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

#include "stb_image.h"

namespace Eis
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (path.find("C:/") != std::string::npos)
			EIS_CORE_WARN("Absolute path is not going to work on other computers!");

		int width = 0, height = 0, channels = 0;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			EIS_PROFILE_RENDERER_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		EIS_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		EIS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const Image& image) : m_Width(image.GetWidth()), m_Height(image.GetHeight())
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		if (image.GetChannels() != 3 && image.GetChannels() != 4)
			EIS_CORE_ERROR("Only RGB and RGBA image formats are supported!");

		m_InternalFormat = image.GetChannels() == 4 ? GL_RGBA8 : GL_RGB8;
		m_DataFormat = image.GetChannels() == 4 ? GL_RGBA : GL_RGB;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, image.GetData());
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		EIS_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must cover entire texture!");
		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindTextureUnit(slot, m_RendererId);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glBindTextureUnit(slot, 0);
	}
}