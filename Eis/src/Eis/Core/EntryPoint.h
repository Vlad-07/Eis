#pragma once


#ifdef EIS_PLATFORM_WINDOWS

extern Eis::Application* Eis::CreateApplication();

int main(int argc, char** args)
{
	Eis::Log::Init();
	EIS_CORE_TRACE("Init");

	EIS_PROFILE_BEGIN_SESSION("Startup", "EisProfile-Startup.json");
	auto app = Eis::CreateApplication();
	EIS_PROFILE_END_SESSION();

	EIS_PROFILE_BEGIN_SESSION("Runtime", "EisProfile-Runtime.json");
	app->Run();
	EIS_PROFILE_END_SESSION();

	EIS_PROFILE_BEGIN_SESSION("Shutdown", "EisProfile-Shutdown.json");
	delete app;
	EIS_PROFILE_END_SESSION();

	return 0;
}

#endif