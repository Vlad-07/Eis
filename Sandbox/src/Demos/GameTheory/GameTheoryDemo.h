#pragma once

#include <Eis.h>
#include <imgui.h>
#include <implot.h>

#include "Demos/Demo.h"

#include "GameTheory.h"
#include "Simulation.h"

class GTDemo : public Demo
{
public:
	GTDemo(const std::string& name);

	virtual void Init() override;
	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event&) override;

private:
	GameTheory::Simulation m_Simulation;
	std::vector<float> m_xHistory, m_yHistory;
	Eis::TimeStep lastTs;
};