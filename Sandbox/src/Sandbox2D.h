#pragma once

#include "Eis.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

class Sandbox2D : public Eis::Layer
{
private:
	Eis::Ref<Eis::VertexArray> m_VA;
	Eis::Ref<Eis::Shader> m_FlatColorShader;
	Eis::OrthoCameraController m_CameraController;


	int frame = 0;

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Eis::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Eis::Event& e) override;
};