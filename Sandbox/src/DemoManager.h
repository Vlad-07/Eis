#pragma once

#include <Eis.h>

#include "Demos/Demo.h"

class DemoManager
{
public:
	DemoManager() : m_CurrentDemoId(0), m_Demos() {}
	~DemoManager() { for (Demo*& p : m_Demos) delete p; }

	void LoadDemo(Demo* demo) { m_Demos.push_back(demo); }

	void SetCurrentDemo(int id) { m_CurrentDemoId = id; }

	Demo& GetCurrentDemo() { return *m_Demos[m_CurrentDemoId]; }
	const std::vector<Demo*> GetDemos() const { return m_Demos; }
	int GetCurrentDemoId() const { return m_CurrentDemoId; }

private:
	std::vector<Demo*> m_Demos;
	int m_CurrentDemoId;
};