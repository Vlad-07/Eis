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
		int succes = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EIS_CORE_ASSERT(succes, "Faield to initialize Glad!");

		EIS_CORE_INFO("OpenGL Renderer:" );
		EIS_CORE_INFO("    Vendor:   {0}", glGetString(GL_VENDOR));
		EIS_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
		EIS_CORE_INFO("    Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}