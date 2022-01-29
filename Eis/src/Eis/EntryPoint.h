#pragma once


#ifdef EIS_PLATFORM_WINDOWS


extern Eis::Application* Eis::CreateApplication();

int main(int argc, char** args)
{
	Eis::Log::Init();
	EIS_CORE_TRACE("Init");

	auto app = Eis::CreateApplication();

	app->Run();

	delete app;
	return 0;
}

#endif