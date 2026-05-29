#include "Eispch.h"
#include "WebGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Eis
{
	WebGLContext::WebGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		EIS_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void WebGLContext::Init()
	{
		EIS_PROFILE_FUNCTION();

		if (gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress) == 0)
		{
			EIS_CORE_CRITICAL("Failed to initialize Glad!");
			std::exit(1);
		}

		EIS_CORE_INFO("WebGL Renderer:");
		EIS_CORE_INFO("    Vendor:  {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		EIS_CORE_INFO("    Device:  {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		EIS_CORE_INFO("    Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	void WebGLContext::SwapBuffers()
	{
		EIS_PROFILE_FUNCTION();

		// not necessary for web
		// See EmscriptenWebGLContextAttributes.explicitSwapControl
		glfwSwapBuffers(m_WindowHandle);
	}
}