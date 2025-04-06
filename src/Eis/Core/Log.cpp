#include "Eispch.h"
#include "Log.h"


namespace Eis
{
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
	#ifdef EIS_LOG_FULL
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks[0]->set_pattern("%^[%T] %n:%$ %v");

		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Eis.log", true));
		logSinks[1]->set_pattern("[%T] %n: [%L] %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("EIS", logSinks.begin(), logSinks.end());
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", logSinks.begin(), logSinks.end());
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	#elif defined(EIS_LOG_NOFILE)
		auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
		sink->set_pattern("[%T] %n: [%L] %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("EIS", sink);
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", sink);
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	#else
		#error Unknown log level!
	#endif
	}
}