#pragma once


namespace Eis
{
	class Application;
	class Window;
	class Input;

	class EisContext
	{
	public:
		EisContext() = default;
		~EisContext() = default;

		Application& App() { return *m_App; }
		Window& Window() { return *m_Window; }
		Input& Input() { return *m_Input; }

	private:
		// Tehnically unsafe

		Eis::Application* m_App = nullptr;
		Eis::Window* m_Window = nullptr;
		Eis::Input* m_Input = nullptr;

		friend class Application;
	};
}