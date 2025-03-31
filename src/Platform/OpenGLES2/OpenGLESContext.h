#pragma once

#include "Eis/Rendering/GraphicsContext.h"


struct GLFWwindow;

namespace Eis
{
	class OpenGLESContext : public GraphicsContext
	{
	public:
		OpenGLESContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}