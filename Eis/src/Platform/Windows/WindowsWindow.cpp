#include "Eispch.h"
#include "WindowsWindow.h"

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

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		EIS_TRACE("Initialized '{0}' window ({1}, {2}, {3})", m_Data.Title, m_Data.Width, m_Data.Height, m_Data.VSync);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		EIS_TRACE("Destroyed '{0}' window", m_Data.Title);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
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