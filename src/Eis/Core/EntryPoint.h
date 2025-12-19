#pragma once

#include "Eis/Core/Core.h"
#include "Eis/Debug/Instrumentor.h"


#if defined(EIS_PLATFORM_WINDOWS) || defined(EIS_PLATFORM_WEB)

extern Eis::Application* Eis::CreateApplication();

int main(int argc, char** args)
{
	EIS_PROFILE_BEGIN_SESSION("Startup", "EisProfile-Startup.json");
	Eis::Log::Init();
	EIS_CORE_TRACE("Init");

	auto app = Eis::CreateApplication();
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