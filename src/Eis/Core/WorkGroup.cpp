#include <Eispch.h>
//#include "WorkGroup.h"
//
//
//namespace Eis
//{
//	WorkGroup::~WorkGroup()
//	{
//		JoinAll();
//	}
//
//	void WorkGroup::Dispatch()
//	{
//		for (uint8_t i = 0; i < m_WorkerNr; i++)
//			m_Workers.emplace_back(m_Tasks[i]);
//	}
//
//	void WorkGroup::JoinAll()
//	{
//		for (std::thread& t : m_Workers)
//			if (t.joinable())
//				t.join();
//		m_Workers.clear();
//	}
//}