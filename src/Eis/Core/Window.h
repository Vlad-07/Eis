#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "Eis/Core/Core.h"
#include "Eis/Input/Input.h"
#include "Eis/Events/Event.h"


namespace Eis
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Default Window", uint32_t width = 0, uint32_t height = 0)
			: Title(title), Width(width), Height(height) {}

		inline static constexpr uint32_t DefaultWidth = 1280;
		inline static constexpr uint32_t DefaultHeight= 720;
	};


	// Desktop system based window interface
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }
		glm::fvec2 GetScale() const { return m_Data.Scale; }
		bool IsVSync() const { return m_Data.VSync; }
		bool IsFocused() const { return m_Data.Focused; }
		bool IsIconified() const { return m_Data.Iconified; }

		virtual void SetSize(uint32_t width, uint32_t height) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

		Input& GetInput() { return m_Input; }

		static Scope<Window> Create(const WindowProps& props = WindowProps());

	protected:
		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			glm::fvec2 Scale;
			bool Focused, Iconified;
			bool VSync;

			EventCallbackFn EventCallback;
		} m_Data;

		Input m_Input;

	protected:
		// Maybe handle this using a window manager?
		inline static uint8_t s_WindowCount = 0;
	};
}