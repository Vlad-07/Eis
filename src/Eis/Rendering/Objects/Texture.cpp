#include "Eispch.h"
#include "Texture.h"

#include "Eis/Rendering/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLTexture.h"


namespace Eis
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "Invalid graphics API: None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "Invalid graphics API: None");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const Image& image)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "RendererAPI::None not currently suported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(image);
		}

		EIS_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}
}