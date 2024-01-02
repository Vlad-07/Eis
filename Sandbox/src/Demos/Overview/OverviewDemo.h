#pragma once

#include "Eis.h"

#include <imgui.h>
#include <implot.h>

#include "Demos/Demo.h"

class OverviewDemo : public Demo
{
public:
	OverviewDemo(const std::string& name);
	~OverviewDemo() = default;

	virtual void Init() override;
	virtual void Update(Eis::TimeStep ts) override;
	virtual void ImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CameraController;
	Eis::Ref<Eis::Texture2D> ice, mouce;
	float m_LineAngle;
};