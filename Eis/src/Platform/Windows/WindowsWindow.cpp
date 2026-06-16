#include "Eispch.h"
#include "WindowsWindow.h"

#include "Eis/Events/KeyEvent.h"
#include "Eis/Events/MouseEvent.h"
#include "Eis/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>


#ifdef EIS_PLATFORM_WINDOWS


namespace Eis
{
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		EIS_PROFILE_FUNCTION();

		// Init GLFW

		if (s_WindowCount == 0)
		{
			EIS_PROFILE_SCOPE("glfwInit");
			EIS_CORE_INFO("Initializing GLFW...");

			int succes = glfwInit();
			EIS_CORE_ASSERT(succes, "Could not initialize GLFW!");

			glfwSetErrorCallback([](int error_code, const char* description)
			{
				EIS_CORE_ERROR("OpenGL Error: {} ({})", error_code, description);
			});
		}

		// Init Window

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_MAXIMIZED, true);

		{
			EIS_PROFILE_SCOPE("glfwCreateWindow");

			m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

			EIS_CORE_ASSERT(m_Window, "Failed to create {} window!", m_Data.Title);

			s_WindowCount++;
		}

		// Init context

		glfwMakeContextCurrent(m_Window);

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();


		// Set window settings

		SetVSync(m_Data.VSync);

		glfwGetFramebufferSize(m_Window, (int*)&m_Data.Width, (int*)&m_Data.Height); // Ensure correct size on high dpi displays
		glfwGetWindowContentScale(m_Window, &m_Data.Scale.x, &m_Data.Scale.y);

		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetFramebufferSizeCallback(m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});
		glfwSetWindowContentScaleCallback(m_Window,
			[](GLFWwindow* window, float xScale, float yScale)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Scale.x = xScale;
				data.Scale.y = yScale;

				WindowRescaleEvent event(xScale, yScale);
				data.EventCallback(event);
			});
		glfwSetWindowPosCallback(m_Window,
			[](GLFWwindow* window, int xpos, int ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowMovedEvent event(xpos, ypos);
				data.EventCallback(event);
			});
		glfwSetWindowFocusCallback(m_Window,
			[](GLFWwindow* window, int focused)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Focused = static_cast<bool>(focused);

				if (data.Focused)
				{
					WindowFocusEvent event;
					data.EventCallback(event);
				}
				else
				{
					WindowLostFocusEvent event;
					data.EventCallback(event);
				}
			});
		glfwSetWindowIconifyCallback(m_Window,
			[](GLFWwindow* window, int iconified)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Iconified = static_cast<bool>(iconified);

				if (data.Iconified)
				{
					WindowIconifyEvent event;
					data.EventCallback(event);
				}
				else
				{
					WindowDeiconifyEvent event;
					data.EventCallback(event);
				}
			});
		glfwSetWindowCloseCallback(m_Window,
			[](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window,
			[](GLFWwindow* window, int key, int scanCode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						return;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						return;
					}
					case GLFW_REPEAT:
					{
						// GLFW does not provide a way to get the repeat count. It is possible to extract it but I have lazy
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						return;
					}
					default:
						EIS_CORE_ASSERT(false, "Invalid key action! (glfwSetKeyCallback)");
				}
			});
		glfwSetCharCallback(m_Window,
			[](GLFWwindow* window, unsigned int key)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(key);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window,
			[](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event(static_cast<float>(xpos) * data.Scale.x, static_cast<float>(xpos)* data.Scale.y);
				data.EventCallback(event);
			});
		glfwSetMouseButtonCallback(m_Window,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						return;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						return;
					}
					default:
						EIS_CORE_ASSERT(false, "Invalid mouse action! (glfwSetMouseCallback)");
				}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
				data.EventCallback(event);
			});
	}

	WindowsWindow::~WindowsWindow()
	{
		EIS_PROFILE_FUNCTION();

		EIS_CORE_TRACE("Destroyed '{}' window", m_Data.Title);
		glfwDestroyWindow(m_Window);
		s_WindowCount--;

		if (s_WindowCount == 0)
			glfwTerminate();
	}

	void WindowsWindow::PollEvents()
	{
		EIS_PROFILE_FUNCTION();

		if (m_Data.Focused)
			glfwPollEvents();
		else
			glfwWaitEventsTimeout(1.0f / m_Data.UnfocusedMinFPS);
	}

	void WindowsWindow::SwapBuffers()
	{
		EIS_PROFILE_FUNCTION();

		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetSize(uint32_t width, uint32_t height)
	{
		EIS_PROFILE_FUNCTION();

		glfwSetWindowSize(m_Window, width, height);
		m_Data.Width = width;
		m_Data.Height = height;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		EIS_PROFILE_FUNCTION();

		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		EIS_PROFILE_FUNCTION();

		glfwSetWindowTitle(m_Window, title.c_str());
		m_Data.Title = title;
	}
}

#endif // EIS_PLATFORM_WINDOWS