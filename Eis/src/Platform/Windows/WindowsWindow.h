#pragma once

#include "Eis/Core/Window.h"
#include "Eis/Rendering/GraphicsContext.h"


struct GLFWwindow;

namespace Eis
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual void SetSize(uint32_t width, uint32_t height) override;
		virtual void SetVSync(bool enabled) override;
		virtual void SetTitle(const std::string& title) override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		virtual void* GetNativeWindow() const override { return static_cast<void*>(m_Window); }

	private:
		GLFWwindow* m_Window = nullptr;
		Scope<GraphicsContext> m_Context;
	};
}