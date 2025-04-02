#pragma once

#include "Eis/Rendering/GraphicsContext.h"


struct GLFWwindow;

namespace Eis
{
	class OpenGLES2Context : public GraphicsContext
	{
	public:
		OpenGLES2Context(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override {}

	private:
		GLFWwindow* m_WindowHandle;
	};
}