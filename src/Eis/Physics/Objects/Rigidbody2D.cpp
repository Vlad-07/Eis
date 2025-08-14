#include <Eispch.h>
#include "Rigidbody2D.h"
#include <glm/gtc/constants.hpp>
#include <Eis/Rendering/Renderer/Renderer2D.h>
#include <glm/gtc/matrix_transform.hpp>


namespace Eis
{
	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float radius, float density, float restitution, bool isStatic)
		: m_Position(pos)
	{
		m_Properties.Type = RigidbodyType2D::CIRCLE;
		m_Properties.Density = density;
		m_Properties.Radius = radius;
		m_Properties.Restitution = restitution;
		m_Properties.Size = glm::vec2();
		m_Properties.Area = radius * radius * glm::pi<float>();
		m_Properties.Mass = m_Properties.Area * density;
		m_Properties.InvMass = !isStatic ? 1.0f / m_Properties.Mass : 0.0f;
		m_Properties.Static = isStatic;
	}

	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic)
		: m_Position(pos), m_Rotation(rotation)
	{
		m_Properties.Type = RigidbodyType2D::BOX;
		m_Properties.Density = density;
		m_Properties.Size = size;
		m_Properties.Restitution = restitution;
		m_Properties.Radius = 0.0f;
		m_Properties.Area = size.x * size.y;
		m_Properties.Mass = m_Properties.Area * density;
		m_Properties.InvMass = !isStatic ? 1.0f / m_Properties.Mass : 0.0f;
		m_Properties.Static = isStatic;

		const glm::vec2 botL = { -size.x / 2.0f, -size.y / 2.0f };
		const glm::vec2 botR = { size.x / 2.0f, -size.y / 2.0f };
		const glm::vec2 topR = { size.x / 2.0f,  size.y / 2.0f };
		const glm::vec2 topL = { -size.x / 2.0f,  size.y / 2.0f };
		m_Properties.Vertices = { botL, botR, topR, topL };
		CalculateTransformedVertices();
	}


	void Rigidbody2D::Update(const TimeStep& ts, const glm::vec2 gravity)
	{
		if (m_Properties.Static) return;

		/*m_Position += m_LinearVelocity * ts + m_Acceleration * (ts * ts * 0.5f);

		const glm::vec2 newAccel = gravity + m_Force * m_Properties.InvMass;
		m_Force = glm::vec2(0.0f);

		m_LinearVelocity += (m_Acceleration + newAccel) * (ts * 0.5f);

		m_Acceleration = newAccel;//*/

		m_LinearVelocity += ((m_Force * m_Properties.InvMass) + gravity) * (float)ts;
		m_Force = glm::vec2();

		m_Position += m_LinearVelocity * (float)ts;
	}


	const ColliderVertices2D& Rigidbody2D::GetTransformedVertices() const
	{
		if (!m_Properties.Static)
			CalculateTransformedVertices();

		return m_TransformedVertices;
	}

	void Rigidbody2D::CalculateTransformedVertices() const
	{
		const float sin = glm::sin(m_Rotation);
		const float cos = glm::cos(m_Rotation);

		for (uint8_t i = 0; i < m_Properties.Vertices.length(); i++)
		{
			const glm::vec2& vertex = m_Properties.Vertices[i];
			m_TransformedVertices[i] = { cos * vertex.x - sin * vertex.y + m_Position.x,
										sin * vertex.x - cos * vertex.y + m_Position.y };
		}
	}


	void Rigidbody2D::Draw(const glm::vec4& color) const
	{
		switch (m_Properties.Type)
		{
		case RigidbodyType2D::CIRCLE:
			Renderer2D::DrawCircle(m_Position, glm::vec2(m_Properties.Radius * 2.0f), color);
			break;

		case RigidbodyType2D::BOX:
			Renderer2D::DrawRotatedQuad(m_Position, m_Properties.Size, m_Rotation, color);
			break;

		case RigidbodyType2D::NONE:
		default:
			EIS_ERROR("Invalid Rigidbody2D type!");
			break;
		}
	}

	void Rigidbody2D::Draw(Ref<Texture2D> tex) const
	{
		switch (m_Properties.Type)
		{
		case RigidbodyType2D::CIRCLE:
			EIS_ERROR("Cannot draw textured circles!");
			break;

		case RigidbodyType2D::BOX:
			Renderer2D::DrawRotatedQuad(m_Position, m_Properties.Size, m_Rotation, tex);
			break;

		case RigidbodyType2D::NONE:
		default:
			EIS_ERROR("Invalid Rigidbody2D type!");
			break;
		}
	}
}