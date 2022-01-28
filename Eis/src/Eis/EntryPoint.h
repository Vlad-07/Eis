#pragma once

#ifdef EIS_PLATFORM_WINDOWS


// Ignore errors
extern Eis::Application* Eis::CreateApplication();

int main(int argc, char** args)
{
	auto app = Eis::CreateApplication();

	app->Run();

	delete app;
	return 0;
}

#endif