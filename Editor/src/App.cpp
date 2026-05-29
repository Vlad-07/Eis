#include <Eis/Core/Application.h>
#include <Eis/Core/EntryPoint.h>

#include "EditorLayer.h"


namespace Eis
{
	class EditorApp : public Application
	{
	public:
		EditorApp(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(CreateScope<EditorLayer>());
		}

		virtual ~EditorApp() = default;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Editor";
		spec.CommandLineArgs = args;
	
		return new EditorApp(spec);
	}
}