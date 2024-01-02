#pragma once

#include "GameTheory.h"

#include <vector>
#include <array>

#include <Eis.h>


namespace GameTheory
{
	static const int TableNumber		= 150;
	static const int StartSubjectNumber	= 100;
	static const int MaxSubjects = TableNumber * 2;
	static const int MaxPickRetries		= 100;


	class Subject
	{
	public:
		Subject() = default;
		Subject(Strategy s) : m_Strategy(s), m_Handeled(false), m_Score(0.0f) {}
		~Subject() = default;

		Strategy GetStrategy() const { return m_Strategy; }
		void SetStrategy(Strategy s) { m_Strategy = s; }

		bool IsHandeled() const { return m_Handeled; }
		void SetHandeled(bool h) { m_Handeled = h; }

		float GetScore() const { return m_Score; }
		void SetScore(float s) { m_Score = s; }

	private:
		Strategy m_Strategy;
		float m_Score;
		bool m_Handeled;
	};


	class Table
	{
	public:
		Table() : m_Id1(EmptyId), m_Id2(EmptyId) {}
		~Table() = default;

		void SetInEmptySpot(int id) { if (m_Id1 == EmptyId) m_Id1 = id; else if (m_Id2 == EmptyId) m_Id2 = id; else EIS_ASSERT(false, "Full table!"); }
		void Reset() { m_Id1 = m_Id2 = EmptyId; }

		int GetId1() const { return m_Id1; }
		int GetId2() const { return m_Id2; }
		bool IsAvailable() const { return (m_Id1 == EmptyId) || (m_Id2 == EmptyId); }
		bool IsEmpty() const { return (m_Id1 == EmptyId) && (m_Id2 == EmptyId); }

	private:
		int m_Id1, m_Id2;
		static const int EmptyId = -1;
	};


	class Simulation
	{
	public:
		Simulation() = default;
		~Simulation() = default;

		void Init();
		void Reset();

		void Step();

		void SetMatrix(RewardMatrix mat) { m_RewardMatrix = mat; }
		RewardMatrix GetMatrix() { return m_RewardMatrix; }
		const RewardMatrix GetMatrix() const { return m_RewardMatrix; }

		int GetStep() const { return m_Step; }
		int GetTypeCount(Strategy strat) const;

		std::vector<Subject>& GetSubjects() { return m_Subjects; }
		const std::vector<Subject>& GetSubjects() const { return m_Subjects; }

	private:
		int PickRandUnhandeledSubj() const;
		int PickAvailableTable() const;

	private:
		std::vector<Subject> m_Subjects;
		std::array<Table, TableNumber> m_Tables;
		RewardMatrix m_RewardMatrix;
		int m_Step;
	};
}