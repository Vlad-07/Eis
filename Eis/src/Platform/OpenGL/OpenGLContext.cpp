#include "Eispch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Eis
{
	namespace
	{
		std::string GLErrSrcToString(GLenum source)
		{
			switch (source)
			{
				case GL_DEBUG_SOURCE_API:             return "Source: API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Source: Window System";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Source: Shader Compiler";
				case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Source: Third Party";
				case GL_DEBUG_SOURCE_APPLICATION:     return "Source: Application";
				case GL_DEBUG_SOURCE_OTHER:           return "Source: Other";
				default: EIS_CORE_ASSERT(false) return "";
			}
		}

		std::string GLErrTypeToString(GLenum type)
		{
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR:               return "Type: Error";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Type: Deprecated Behaviour";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Type: Undefined Behaviour";
				case GL_DEBUG_TYPE_PORTABILITY:         return "Type: Portability";
				case GL_DEBUG_TYPE_PERFORMANCE:         return "Type: Performance";
				case GL_DEBUG_TYPE_MARKER:              return "Type: Marker";
				case GL_DEBUG_TYPE_PUSH_GROUP:          return "Type: Push Group";
				case GL_DEBUG_TYPE_POP_GROUP:           return "Type: Pop Group";
				case GL_DEBUG_TYPE_OTHER:               return "Type: Other";
				default: EIS_CORE_ASSERT(false) return "";
			}
		}

		std::string GLErrSeverityToString(GLenum severity)
		{
			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH:         return "Severity: high";
				case GL_DEBUG_SEVERITY_MEDIUM:       return "Severity: medium";
				case GL_DEBUG_SEVERITY_LOW:          return "Severity: low";
				case GL_DEBUG_SEVERITY_NOTIFICATION: return "Severity: notification";
				default: EIS_CORE_ASSERT(false) return "";
			}
		}

		void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
			// non-significant error/warning codes: 131169, 131185, 131218, 131204
			if (id == 131185) return;

			std::string_view glMessage{ message, (size_t)length };
			std::string msg = "OpenGL: (" + std::to_string(id) + ") "
				+ std::string{ glMessage } + " "
				+ GLErrTypeToString(type) + " "
				+ GLErrSrcToString(source) + " "
				+ GLErrSeverityToString(severity);

			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH:
					EIS_CORE_ERROR(msg);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					EIS_CORE_WARN(msg);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					EIS_CORE_INFO(msg);
					break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					EIS_CORE_TRACE(msg);
					break;

				default: EIS_CORE_ERROR("Unknown severity!"); break;
			}
		}
	}


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
			EIS_CORE_CRITICAL("OpenGL version 4.5+ is required!");

		GLint flags{};
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLErrorCallback, nullptr);

			EIS_CORE_INFO("OpenGL debug context enabled");
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		EIS_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}