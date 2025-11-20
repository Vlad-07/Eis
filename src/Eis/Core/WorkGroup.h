#pragma once

#include <thread>
#include <functional>


namespace Eis
{
	// TOOD: NOT FINISHED, probably not working
	// helper and TODO: compatibility with web
	class WorkGroup
	{
	public:
		WorkGroup(uint8_t workers) : m_WorkerNr(workers), m_Tasks(workers) {}
		~WorkGroup();
		
		template<typename F, typename ...Args>
		inline void SetTask(uint8_t workerId, F&& f, Args&&... args)
		{
			m_Tasks[workerId] = [f, args...]() { f(args...); };
		}

		void Dispatch();
		void JoinAll();

		uint8_t GetWorkerNr() const { return m_WorkerNr; }

	private:
		std::vector<std::function<void()>> m_Tasks;
		std::vector<std::thread> m_Workers;
		uint8_t m_WorkerNr;
	};
}