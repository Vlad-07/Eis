#include <Eis/Core/Application.h>
#include <Eis/Core/EntryPoint.h>

#include "EditorLayer.h"


namespace Eis
{
	class EditorApp : public Application
	{
	public:
		EditorApp()
		{
			RegisterLayer<EditorLayer>("EditorLayer");
		}

		virtual ~EditorApp() = default;
	};

	Application* CreateApplication()
	{
		return new EditorApp();
	}
}