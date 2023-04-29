#pragma once

#include "Eis.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

class Sandbox2D : public Eis::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Eis::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;

private:
	Eis::OrthoCameraController m_CameraController;
	Eis::Ref<Eis::Texture2D> ice, map, mouce;
};