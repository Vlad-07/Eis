#include "Eispch.h"
#include "Shader.h"

#include "Eis/Renderer/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Eis
{
	Shader* Shader::Create(std::string& vsSrc, std::string& fsSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EIS_CORE_ASSERT(false, "RendererAPI::None not currently suported");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vsSrc, fsSrc);
		}

		EIS_CORE_ASSERT(false, "Unknown API");
		return nullptr;
	}
}