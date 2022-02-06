#include "Eispch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Eis
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		EIS_CORE_ASSERT(windowHandle, "Window handle is null!!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int  succes = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EIS_CORE_ASSERT(succes, "Faield to initialize Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}
}