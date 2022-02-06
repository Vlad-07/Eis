#include "Eispch.h"
#include "WindowsWindow.h"

#include "Eis/Events/ApplicationEvent.h"
#include "Eis/Events/KeyEvent.h"
#include "Eis/Events/MouseEvent.h"

#include <glad/glad.h>


namespace Eis
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on sys shutdown
			int succes = glfwInit();
			EIS_CORE_ASSERT(succes, "Could not initialize GLFW!");

			glfwSetErrorCallback([](int error_code, const char* description)
			{
					EIS_CORE_CRITICAL("Open GL error ({0}): {1}", error_code, description);
			});

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		int  succes = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EIS_CORE_ASSERT(succes, "Faield to initialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		EIS_TRACE("Initialized '{0}' window ({1}, {2}, {3})", m_Data.Title, m_Data.Width, m_Data.Height, m_Data.VSync);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowResizeEvent event(width, height);
			
				data.Width = width;
				data.Height = height;
				data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanCode, int action, int mods)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1); // GLFW does not provide a way to get the repeat count. It is possible to extract it but I have lazy
						data.EventCallback(event);
						break;
					}
					default:
						EIS_CORE_ASSERT(false, "HOW DID THIS HAPPEN!!! WindowsWindow.cpp:104-ish");
				}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
		});
		
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				default:
					EIS_CORE_ASSERT(false, "HOW DID THIS HAPPEN!!! WindowsWindow.cpp:141-ish");
				}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		EIS_TRACE("Destroyed '{0}' window", m_Data.Title);
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);

		glClearColor(0.1f, 0.7f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}