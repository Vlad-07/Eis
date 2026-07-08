#pragma once

#include "Core.h"

// maybe move define to build system?
#define SPDLOG_COMPILED_LIB
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <glm/gtx/string_cast.hpp>


int main(int argc, char** args);

namespace Eis
{
	class Log
	{
	public:
		static Log& Get() { static Log log; return log; }

		static Ref<spdlog::logger>& GetCoreLogger() { return Get().m_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return Get().m_ClientLogger; }

	private:
		void Init();

		friend int ::main(int argc, char** args);

	private:
		Ref<spdlog::logger> m_CoreLogger;
		Ref<spdlog::logger> m_ClientLogger;
	};
}

// TODO: try to remove spdlog include dependency
// Consider stdarg.h over compiler specific "__VA_ARGS__" ?

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