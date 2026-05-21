#include "Eispch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Eis
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		EIS_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		EIS_PROFILE_FUNCTION();

		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
		{
			EIS_CORE_CRITICAL("Failed to initialize Glad!");
			std::exit(1);
		}

		EIS_CORE_INFO("OpenGL Renderer:");
		EIS_CORE_INFO("    Vendor:  {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		EIS_CORE_INFO("    Device:  {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		EIS_CORE_INFO("    Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		if (!(GLVersion.major == 4 && GLVersion.minor >= 5))
			EIS_CORE_CRITICAL("OpenGL version 4.5 or newer is required!");
	}

	void OpenGLContext::SwapBuffers()
	{
		EIS_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}