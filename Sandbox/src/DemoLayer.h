#pragma once

#include "Eis.h"

#include <imgui.h>
#include <implot.h>

#include "DemoManager.h"
#include "Demos/Overview/OverviewDemo.h"
#include "Demos/GameTheory/GameTheoryDemo.h"


class DemoLayer : public Eis::Layer
{
public:
	DemoLayer();
	virtual ~DemoLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Eis::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	DemoManager m_DemoManager;
};