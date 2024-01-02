#include "Simulation.h"

#include <Eis/Core/Random.h>

void GameTheory::Simulation::Init()
{
	m_Step = 0;

	for (int i = 0; i < StartSubjectNumber; i++)
		m_Subjects.push_back(GameTheory::Subject(Eis::Random::UInt() % 2 == 0 ? GameTheory::A : GameTheory::B));
}
void GameTheory::Simulation::Reset()
{
	m_Subjects.clear();
	Init();
}


void GameTheory::Simulation::Step()
{
	m_Step++;

	// Chose a table for each player
	for (int i = 0; i < m_Subjects.size(); i++)
		m_Tables[PickAvailableTable()].SetInEmptySpot(i);

	// Handle scores and reset tables
	for (Table& t : m_Tables)
	{
		if (t.IsEmpty())
			continue;

		if (t.GetId1() == -1)
			m_Subjects[t.GetId2()].SetScore(m_RewardMatrix.GetDefaultReward());
		else if (t.GetId2() == -1)
			m_Subjects[t.GetId1()].SetScore(m_RewardMatrix.GetDefaultReward());
		else
		{
			m_Subjects[t.GetId1()].SetScore(m_RewardMatrix.GetReward(m_Subjects[t.GetId1()].GetStrategy(), m_Subjects[t.GetId2()].GetStrategy()));
			m_Subjects[t.GetId2()].SetScore(m_RewardMatrix.GetReward(m_Subjects[t.GetId2()].GetStrategy(), m_Subjects[t.GetId1()].GetStrategy()));
		}
		
		t.Reset();
	}

	// Selection
	int addA = 0, addB = 0;
	for (int i = 0; i < m_Subjects.size(); i++)
	{
		Subject& s = m_Subjects[i];
		float poll = Eis::Random::Float();

		if (poll < s.GetScore() / 2.0f) // Reproduce
		{
			if (s.GetStrategy() == A) addA++;
			else addB++;
		}
		else if (poll > s.GetScore()) // Die
			m_Subjects.erase(m_Subjects.begin() + i), i--;
		// Survival is between score / 2 and score
	}

	// Add offsprings
	for (int i = 0; i < addA && m_Subjects.size() < MaxSubjects; i++)
		m_Subjects.push_back(Subject(A));
	for (int i = 0; i < addB && m_Subjects.size() < MaxSubjects; i++)
		m_Subjects.push_back(Subject(B));
}


int GameTheory::Simulation::GetTypeCount(Strategy strat) const
{
	int count = 0;
	for (Subject s : m_Subjects)
		if (s.GetStrategy() == strat)
			count++;
	return count;
}


int GameTheory::Simulation::PickRandUnhandeledSubj() const
{
	int id = -1;

	for (int retry = 0; id == -1 && retry < MaxPickRetries; retry++)
	{
		id = Eis::Random::UInt(0, m_Subjects.size() - 1);
		if (m_Subjects[id].IsHandeled()) id = -1;
	}

	for (int i = 0; id == -1 && i < m_Subjects.size(); i++)
		if (!m_Subjects[i].IsHandeled())
			id = i;

	if (id == -1)
		EIS_ASSERT(false, "All subjects handeled!");

	return id;
}
int GameTheory::Simulation::PickAvailableTable() const
{
	int t = -1;
	for (int retry = 0; t == -1 && retry < MaxPickRetries; retry++)
	{
		t = Eis::Random::UInt(0, TableNumber - 1);
		if (!m_Tables[t].IsAvailable()) t = -1;
	}
	for (int i = 0; t == -1 && i < TableNumber; i++)
		if (m_Tables[i].IsAvailable())
			t = i;

	if (t == -1)
		EIS_ASSERT(false, "All tables full!");

	return t;
}