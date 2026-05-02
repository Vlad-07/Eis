#pragma once

#include "Eis/Core/Core.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ios>
#include <string>
#include <chrono>
#include <thread>

// TODO: use tracy

namespace Eis
{
	struct ProfileResult
	{
		std::string Name;
		std::chrono::duration<double, std::micro> Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

		void BeginSession(const std::string& name, const std::string& filePath = "results.json")
		{
			m_OutputStream.open(filePath, std::ios::out | std::ios::trunc);

			if (m_OutputStream.good())
			{
				WriteHeader();
				m_CurrentSession = std::make_shared<InstrumentationSession>(InstrumentationSession{ name });
			}
			else if (Log::GetCoreLogger())
				EIS_CORE_ERROR("Instrumentor could not open results file '{}'!", filePath);
		}
		void EndSession()
		{
			WriteFooter();
			m_ProfileCount = 0;
			m_OutputStream.close();
			m_CurrentSession.reset();
		}


		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ',';

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << '{'
							<< "\"cat\":\"function\","
							<< "\"dur\":" << (result.ElapsedTime.count()) << ','
							<< "\"name\":\"" << name << "\","
							<< "\"ph\":\"X\","
							<< "\"pid\":0,"
							<< "\"tid\":" << result.ThreadID << ','
							<< "\"ts\":" << result.Start.count()
							<< '}';
			m_OutputStream.flush();
		}


		void WriteHeader()
		{
			m_OutputStream << std::fixed << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}
		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		Ref<InstrumentationSession> m_CurrentSession;
		std::ofstream m_OutputStream;
		uint32_t m_ProfileCount;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}


		void Stop()
		{
			const auto endTimepoint = std::chrono::steady_clock::now();
			const auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
									- std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();
			const auto highResStart = std::chrono::duration<double, std::micro>{ m_StartTimepoint.time_since_epoch() };

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}



#ifdef EIS_PROFILE
	#define EIS_PROFILE_BEGIN_SESSION(name, filepath) ::Eis::Instrumentor::Get().BeginSession(name, filepath);
	#define EIS_PROFILE_END_SESSION() ::Eis::Instrumentor::Get().EndSession();
	#define EIS_PROFILE_SCOPE(name) ::Eis::InstrumentationTimer timer##__LINE__(name);
	#define EIS_PROFILE_FUNCTION() EIS_PROFILE_SCOPE(__FUNCSIG__)

#ifdef EIS_PROFILE_RENDERER
	#define EIS_PROFILE_RENDERER_SCOPE(name) EIS_PROFILE_SCOPE(name)
	#define EIS_PROFILE_RENDERER_FUNCTION() EIS_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define EIS_PROFILE_RENDERER_SCOPE(name)
	#define EIS_PROFILE_RENDERER_FUNCTION()
#endif

#else
	#define EIS_PROFILE_BEGIN_SESSION(name, filepath)
	#define EIS_PROFILE_END_SESSION()
	#define EIS_PROFILE_SCOPE(name)
	#define EIS_PROFILE_FUNCTION()
	#define EIS_PROFILE_RENDERER_SCOPE
	#define EIS_PROFILE_RENDERER_FUNCTION()
#endif