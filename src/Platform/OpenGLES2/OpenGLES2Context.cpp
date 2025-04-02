#include "Eispch.h"
#include "OpenGLES2Context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Eis
{
	OpenGLES2Context::OpenGLES2Context(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		EIS_CORE_ASSERT(windowHandle, "Window handle is null!!")
	}

	void OpenGLES2Context::Init()
	{
		EIS_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
		{
			EIS_CORE_CRITICAL("Faield to initialize Glad!");
			std::exit(1);
		}

		EIS_CORE_INFO("OpenGL ES Renderer:" );
		EIS_CORE_INFO("    Vendor:  {0}", glGetString(GL_VENDOR));
		EIS_CORE_INFO("    Device:  {0}", glGetString(GL_RENDERER));
		EIS_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));

#ifdef EIS_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		EIS_CORE_ASSERT(versionMajor >= 3 && versionMinor >= 0, "OpenGL ES version 3.0 or newer is required!");
#endif
	}
}