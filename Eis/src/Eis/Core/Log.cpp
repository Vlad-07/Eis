#include "Eispch.h"

#include "Eis/Core/Log.h"

namespace Eis
{
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T | %O] %n:%$ %v");

		s_CoreLogger = spdlog::stdout_color_mt("Eis");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}