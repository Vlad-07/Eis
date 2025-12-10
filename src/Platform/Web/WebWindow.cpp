#include "Eispch.h"
#include "WebWindow.h"

#include "Eis/Events/KeyEvent.h"
#include "Eis/Events/MouseEvent.h"
#include "Eis/Events/ApplicationEvent.h"

#ifdef EIS_PLATFORM_WEB

#include <emscripten.h>
#include <emscripten/html5.h>

#include <GLFW/emscripten_glfw3.h>


namespace Eis
{
	WebWindow::WebWindow(const WindowProps& props)
	{
		EIS_PROFILE_FUNCTION();

		// Init

		m_Data.Title = props.Title;
		m_Data.Width = props.Width != 0 ? props.Width : EM_ASM_INT({ return window.innerWidth; });
		m_Data.Height = props.Height != 0 ? props.Height : EM_ASM_INT({ return window.innerHeight; });
		m_Data.VSync = true;

		EIS_CORE_INFO("Initializing '{0}' window ({1}, {2}, {3})", m_Data.Title, m_Data.Width, m_Data.Height, m_Data.VSync);
		
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

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		emscripten::glfw3::SetNextWindowCanvasSelector("#canvas1");

		{
			EIS_PROFILE_SCOPE("glfwCreateWindow");

			m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

			EIS_CORE_ASSERT(m_Window, "Failed to create {0} window!", m_Data.Title);

			s_WindowCount++;
		}

		emscripten::glfw3::MakeCanvasResizable(m_Window, "window", nullptr);

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		SetVSync(true);
		glfwGetFramebufferSize(m_Window, (int*)&m_Data.Width, (int*)&m_Data.Height); // Ensure correct size on high dpi displays
		glfwGetWindowContentScale(m_Window, &m_Data.Scale.x, &m_Data.Scale.y);

		EIS_CORE_INFO("Scale: {0}, {1}", m_Data.Scale.x, m_Data.Scale.y);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		 {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;
			
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		glfwSetWindowContentScaleCallback(m_Window, [](GLFWwindow* window, float xScale, float yScale)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Scale.x = xScale;
			data.Scale.y = yScale;
		});
		//glfwSetWindowCloseCallback(); // See emscripten_set_beforeunload_callback below
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
			MouseMovedEvent event((float)xpos * data.Scale.x, (float)ypos * data.Scale.y);
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



		// Allow the F12 key to bubble up to the browser (open developer tools):
		emscripten::glfw3::AddBrowserKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			return mods == 0 && action == GLFW_PRESS && key == GLFW_KEY_F12;
		});

		emscripten_set_beforeunload_callback((void*) &m_Data, [](int eventType, const void*, void* userData) -> const char*
		{
			WindowData& data = *(WindowData*)userData;
			WindowCloseEvent event;
			data.EventCallback(event);
			return "";
		});
	}

	WebWindow::~WebWindow()
	{
		EIS_PROFILE_FUNCTION();

		EIS_CORE_TRACE("Destroyed '{0}' window", m_Data.Title);
		glfwDestroyWindow(m_Window);
		s_WindowCount--;

		if (s_WindowCount == 0)
			glfwTerminate();
	}

	/*
	void WebWindow::Update()
	{
		EIS_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}*/

	void WebWindow::PollEvents()
	{
		EIS_PROFILE_FUNCTION();

		glfwPollEvents();
	}

	void WebWindow::SwapBuffers()
	{
		EIS_PROFILE_FUNCTION();

		m_Context->SwapBuffers();
	}

	void WebWindow::SetSize(uint32_t width, uint32_t height)
	{
		EIS_PROFILE_FUNCTION();

		glfwSetWindowSize(m_Window, width, height);
		m_Data.Width = width;
		m_Data.Height = height;
	}

	void WebWindow::SetVSync(bool enabled)
	{
		EIS_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	void WebWindow::SetTitle(const std::string& title)
	{
		EIS_PROFILE_FUNCTION();

		glfwSetWindowTitle(m_Window, title.c_str());
		m_Data.Title = title;
	}
}

#endif