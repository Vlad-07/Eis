#include "Eispch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


namespace Eis
{
	void Log::Init()
	{
#ifndef EIS_LOG_NOFILE
		std::array<spdlog::sink_ptr, 2> logSinks;
		logSinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		logSinks[0]->set_pattern("%^[%T] %n:%$ %v");

		logSinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Eis.log", true);
		logSinks[1]->set_pattern("[%T] %n: [%L] %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("EIS", logSinks.begin(), logSinks.end());
		s_ClientLogger = std::make_shared<spdlog::logger>("APP", logSinks.begin(), logSinks.end());

#else
		spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
		sink->set_pattern("[%T] %n: [%L] %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("EIS", sink);
		s_ClientLogger = std::make_shared<spdlog::logger>("APP", sink);
#endif

		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}
}