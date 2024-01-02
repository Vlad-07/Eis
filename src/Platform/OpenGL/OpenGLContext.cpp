#include "Eispch.h"

#include "Platform/OpenGL/OpenGLContext.h"

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
		EIS_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int succes = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EIS_CORE_ASSERT(succes, "Faield to initialize Glad!");

		EIS_CORE_INFO("OpenGL Renderer:" );
		EIS_CORE_INFO("    Vendor:   {0}", glGetString(GL_VENDOR));
		EIS_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
		EIS_CORE_INFO("    Version:  {0}", glGetString(GL_VERSION));

#ifdef EIS_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		EIS_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Eis requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		EIS_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}