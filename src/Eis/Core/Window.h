#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "Eis/Core/Core.h"
#include "Eis/Events/Event.h"


namespace Eis
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Default Window", uint32_t width = 0, uint32_t height = 0) : Title(title), Width(width), Height(height) {}

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

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::fvec2 GetScale() const = 0;
		virtual bool IsVSync() const = 0;
		virtual bool IsFocused() const = 0;
		virtual bool IsIconified() const = 0;

		virtual void SetSize(uint32_t width, uint32_t height) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

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
		};

	protected:
		inline static uint8_t s_WindowCount = 0; // Assuming only one context
	};
}