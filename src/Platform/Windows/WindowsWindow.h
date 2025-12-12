#pragma once

#include "Eis/Core/Window.h"
#include "Eis/Rendering/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Eis
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }
		virtual glm::fvec2 GetScale() const override { return m_Data.Scale; }
		virtual bool IsVSync() const override { return m_Data.VSync; }
		virtual bool IsFocused() const override { return m_Data.Focused; }
		virtual bool IsIconified() const override { return m_Data.Iconified; }

		virtual void SetSize(uint32_t width, uint32_t height) override;
		virtual void SetVSync(bool enabled) override;
		virtual void SetTitle(const std::string& title) override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		GLFWwindow* m_Window = nullptr;
		Scope<GraphicsContext> m_Context;

		WindowData m_Data;
	};
}