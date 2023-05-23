#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

// #include <stdarg.h>
// SUS: Consider over compiler specific "__VA_ARGS__" ?


// Core log macros
#define EIS_CORE_TRACE(...)		::Eis::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define EIS_CORE_INFO(...)		::Eis::Log::GetCoreLogger()->info(__VA_ARGS__);
#define EIS_CORE_WARN(...)		::Eis::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define EIS_CORE_ERROR(...)		::Eis::Log::GetCoreLogger()->error(__VA_ARGS__);
#define EIS_CORE_CRITICAL(...)	::Eis::Log::GetCoreLogger()->critical(__VA_ARGS__);


// Client log macros
#define EIS_TRACE(...)			::Eis::Log::GetClientLogger()->trace(__VA_ARGS__);
#define EIS_INFO(...)			::Eis::Log::GetClientLogger()->info(__VA_ARGS__);
#define EIS_WARN(...)			::Eis::Log::GetClientLogger()->warn(__VA_ARGS__);
#define EIS_ERROR(...)			::Eis::Log::GetClientLogger()->error(__VA_ARGS__);
#define EIS_CRITICAL(...)		::Eis::Log::GetClientLogger()->critical(__VA_ARGS__);


namespace Eis
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}