#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include "Eis/Events/Event.h"


namespace Eis
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Size(width, height) {}

		glm::ivec2 GetSize() const { return m_Size; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Size.x << ", " << m_Size.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		glm::ivec2 m_Size;
	};

	class WindowRescaleEvent : public Event
	{
	public:
		WindowRescaleEvent(float xScale, float yScale)
			: m_Scale(xScale, yScale) {}

		glm::vec2 GetScale() const { return m_Scale; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowRescaleEvent: " << m_Scale.x << ", " << m_Scale.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowRescale)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		glm::vec2 m_Scale;
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int xpos, int ypos)
			: m_Pos(xpos, ypos) {}

		glm::ivec2 GetPos() const { return m_Pos; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_Pos.x << ", " << m_Pos.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		glm::ivec2 m_Pos;
	};


	class WindowIconifiedEvent : public Event
	{
	public:
		WindowIconifiedEvent() = default;

		EVENT_CLASS_TYPE(WindowIconified)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowDeiconifiedEvent : public Event
	{
	public:
		WindowDeiconifiedEvent() = default;

		EVENT_CLASS_TYPE(WindowDeiconified)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class WindowFocusedEvent : public Event
	{
	public:
		WindowFocusedEvent() = default;

		EVENT_CLASS_TYPE(WindowFocused)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	/*
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};//*/
}