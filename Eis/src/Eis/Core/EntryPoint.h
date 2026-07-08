#pragma once

#include "Eis/Core/Application.h"
#include "Eis/Debug/Instrumentor.h"


#if defined(EIS_PLATFORM_WINDOWS) || defined(EIS_PLATFORM_WEB)

extern Eis::Application* Eis::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	EIS_PROFILE_BEGIN_SESSION("Startup", "EisProfile-Startup.json");
	Eis::Log::Get().Init();
	EIS_CORE_TRACE("Init");

	auto app = Eis::CreateApplication({ argc, argv });
	EIS_PROFILE_END_SESSION();


	EIS_PROFILE_BEGIN_SESSION("Runtime", "EisProfile-Runtime.json");
	app->Run();
	EIS_PROFILE_END_SESSION();


	EIS_PROFILE_BEGIN_SESSION("Shutdown", "EisProfile-Shutdown.json");
	EIS_CORE_TRACE("Shutting down...");
	delete app;
	EIS_PROFILE_END_SESSION();

	return 0;
}

#endif