#pragma once


namespace Eis
{
	class Application;
	class Window;

	class EisContext
	{
	public:
		EisContext() = default;
		~EisContext() = default;

		Application& App() { return *m_App; }
		Window& Window() { return *m_Window; }

	private:
		// Tehnically unsafe

		Eis::Application* m_App = nullptr;
		Eis::Window* m_Window = nullptr;

		friend class Application;
	};
}