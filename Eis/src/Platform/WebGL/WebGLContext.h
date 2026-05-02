#pragma once

#include "Eis/Rendering/GraphicsContext.h"


struct GLFWwindow;

namespace Eis
{
	class WebGLContext : public GraphicsContext
	{
	public:
		WebGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}