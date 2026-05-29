#pragma once

#include "Eis/Rendering/Objects/Texture.h"


typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace Eis
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpec& spec, Buffer data = Buffer{});
		virtual ~OpenGLTexture2D();

		virtual const TextureSpec& GetSpec() const override { return m_Spec; }

		virtual uint32_t GetWidth() const override { return m_Spec.Width; }
		virtual uint32_t GetHeight() const override { return m_Spec.Height; }
		virtual uint32_t GetRendererId() const override { return m_RendererId; }

		virtual void SetData(Buffer data) override;

		virtual void Bind(uint32_t slot = 0) const override;


		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererId == other.GetRendererId();
		}

	private:
		TextureSpec m_Spec;

		GLuint m_RendererId{};

		GLenum m_InternalFormat{}, m_DataFormat{};
	};
}