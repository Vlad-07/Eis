#include "Eispch.h"
#include "WindowsWindow.h"

#include "Eis/Events/KeyEvent.h"
#include "Eis/Events/MouseEvent.h"
#include "Eis/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace Eis
{
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		EIS_PROFILE_FUNCTION();

		// Init

		m_Data.Title  = props.Title;
		m_Data.Width  = props.Width  != 0 ? props.Width  : WindowProps::DefaultWidth;
		m_Data.Height = props.Height != 0 ? props.Height : WindowProps::DefaultHeight;
		m_Data.VSync  = true;

		if (s_WindowCount == 0)
		{
			EIS_PROFILE_SCOPE("glfwInit");
			EIS_CORE_INFO("Initializing GLFW...");

			int succes = glfwInit();
			EIS_CORE_ASSERT(succes, "Could not initialize GLFW!");

			glfwSetErrorCallback([](int error_code, const char* description)
			{
				EIS_CORE_ERROR("OpenGL Error: {0} ({1})", error_code, description);
			});
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		{
			EIS_PROFILE_SCOPE("glfwCreateWindow");

			m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

			EIS_CORE_ASSERT(m_Window, "Failed to create {0} window!", m_Data.Title);

			s_WindowCount++;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		SetVSync(true);
		glfwGetFramebufferSize(m_Window, (int*)&m_Data.Width, (int*)&m_Data.Height); // Ensure correct size on high dpi displays
		glfwGetWindowContentScale(m_Window, &m_Data.Scale.x, &m_Data.Scale.y);

		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowResizeEvent event(width, height);

			data.Width = width;
			data.Height = height;
			data.EventCallback(event);
		});
		glfwSetWindowContentScaleCallback(m_Window, [](GLFWwindow* window, float xScale, float yScale)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Scale.x = xScale;
			data.Scale.y = yScale;
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
					KeyPressedEvent event(static_cast<KeyCode>(key), 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(static_cast<KeyCode>(key));
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), 1); // GLFW does not provide a way to get the repeat count. It is possible to extract it but I have lazy
					data.EventCallback(event);
					break;
				}
				default:
					EIS_CORE_ASSERT(false, "Invalid key action! (glfwSetKeyCallback)");
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(static_cast<KeyCode>(keycode));
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
				MouseButtonPressedEvent event(static_cast<MouseCode>(button));
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
				data.EventCallback(event);
				break;
			}
			default:
				EIS_CORE_ASSERT(false, "Invalid mouse action! (glfwSetMouseCallback)");
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
		});
	}

	WindowsWindow::~WindowsWindow()
	{
		EIS_PROFILE_FUNCTION();

		EIS_CORE_TRACE("Destroyed '{0}' window", m_Data.Title);
		glfwDestroyWindow(m_Window);
		s_WindowCount--;

		if (s_WindowCount == 0)
			glfwTerminate();
	}

	/*
	void WindowsWindow::Update()
	{
		EIS_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}*/

	void WindowsWindow::PollEvents()
	{
		EIS_PROFILE_FUNCTION();

		glfwPollEvents();
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

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	void Eis::WindowsWindow::SetTitle(const std::string& title)
	{
		EIS_PROFILE_FUNCTION();

		glfwSetWindowTitle(m_Window, title.c_str());
		m_Data.Title = title;
	}
}