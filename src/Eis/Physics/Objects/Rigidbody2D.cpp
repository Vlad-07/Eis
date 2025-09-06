#include <Eispch.h>
#include "Rigidbody2D.h"
#include <glm/gtc/constants.hpp>
#include <Eis/Rendering/Renderer/Renderer2D.h>


namespace Eis
{
	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float radius, float density, float restitution, bool isStatic)
		: m_Position(pos)
	{
		m_Properties.Type = ColliderType2D::CIRCLE;
		m_Properties.Density = density;
		m_Properties.Radius = radius;
		m_Properties.Restitution = restitution;
		m_Properties.Size = glm::vec2();
		m_Properties.Area = radius * radius * glm::pi<float>();
		m_Properties.Mass = !isStatic ? m_Properties.Area * density : 0.0f;
		m_Properties.InvMass = !isStatic ? 1.0f / m_Properties.Mass : 0.0f;
		m_Properties.AngularInertia = !isStatic ? 0.5f * m_Properties.Mass * m_Properties.Radius * m_Properties.Radius : 0.0f;
		m_Properties.InvAngularInertia = !isStatic ? 1.0f / m_Properties.AngularInertia : 0.0f;
		m_Properties.StaticFriction = 0.6f;
		m_Properties.DynamicFriction = 0.4f;
		m_Properties.Static = isStatic;

		m_UpdateTrVert = false;
		CalculateBoundingBox();
	}

	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic)
		: m_Position(pos), m_Rotation(rotation)
	{
		m_Properties.Type = ColliderType2D::POLYGON;
		m_Properties.Density = density;
		m_Properties.Size = size;
		m_Properties.Restitution = restitution;
		m_Properties.Radius = 0.0f;
		m_Properties.Area = size.x * size.y;
		m_Properties.Mass = !isStatic ? m_Properties.Area * density : 0.0f;
		m_Properties.InvMass = !isStatic ? 1.0f / m_Properties.Mass : 0.0f;
		m_Properties.AngularInertia = !isStatic ? (1.0f / 12.0f) * m_Properties.Mass * (size.x * size.x + size.y * size.y) : 0.0f;
		m_Properties.InvAngularInertia = !isStatic ? 1.0f / m_Properties.AngularInertia : 0.0f;
		m_Properties.StaticFriction = 0.6f;
		m_Properties.DynamicFriction = 0.4f;
		m_Properties.Static = isStatic;

		const glm::vec2 botL = { -size.x / 2.0f, -size.y / 2.0f };
		const glm::vec2 botR = {  size.x / 2.0f, -size.y / 2.0f };
		const glm::vec2 topR = {  size.x / 2.0f,  size.y / 2.0f };
		const glm::vec2 topL = { -size.x / 2.0f,  size.y / 2.0f };
		m_Properties.Vertices = { botL, botR, topR, topL };
		CalculateTransformedVertices();
		CalculateBoundingBox();
	}


	void Rigidbody2D::Update(const TimeStep& ts, const glm::vec2 gravity)
	{
		// Euler method

		if (m_Properties.Static) return;

		m_LinearVelocity += ((m_Force * m_Properties.InvMass) + m_Acceleration + gravity) * (float)ts + m_LinearVelAdd;
		m_AngularVelocity += m_AngVelAdd;

		m_Position += m_LinearVelocity * (float)ts;
		m_Rotation += m_AngularVelocity * (float)ts;

		m_Force = glm::vec2();
		m_Acceleration = glm::vec2();
		m_LinearVelAdd = glm::vec2();
		m_AngVelAdd = 0.0f;

		m_UpdateTrVert = true;
		m_UpdateBB = true;
	}


	const ColliderVertices2D& Rigidbody2D::GetTransformedVertices() const
	{
		CalculateTransformedVertices();
		return m_TransformedVertices;
	}
	void Rigidbody2D::CalculateTransformedVertices() const
	{
		if (!m_UpdateTrVert || m_Properties.Type == ColliderType2D::CIRCLE)
			return;

		m_UpdateTrVert = false;

		const float sin = glm::sin(m_Rotation);
		const float cos = glm::cos(m_Rotation);
		for (uint8_t i = 0; i < m_Properties.Vertices.length(); i++)
		{
			const glm::vec2& vertex = m_Properties.Vertices[i];
			m_TransformedVertices[i] = { cos * vertex.x - sin * vertex.y + m_Position.x,
										 sin * vertex.x + cos * vertex.y + m_Position.y };
		}
	}


	const AlignedBoundingBox2D& Rigidbody2D::GetBoundingBox() const
	{
		CalculateBoundingBox();
		return m_BB;
	}
	void Rigidbody2D::CalculateBoundingBox() const
	{
		if (!m_UpdateBB)
			return;

		m_UpdateBB = false;

		switch (m_Properties.Type)
		{
		case ColliderType2D::CIRCLE:
			m_BB = { m_Position + glm::vec2(-m_Properties.Radius), m_Position + glm::vec2(m_Properties.Radius) };
			break;

		case ColliderType2D::POLYGON:
			CalculateTransformedVertices();
			m_BB = { glm::vec2(std::numeric_limits<float>::max()), -glm::vec2(std::numeric_limits<float>::max()) };
			for (uint8_t i = 0; i < m_TransformedVertices.length(); i++)
			{
				const glm::vec2& v = m_TransformedVertices[i];

				if (v.x < m_BB.BottomLeft.x) m_BB.BottomLeft.x = v.x;
				if (v.y < m_BB.BottomLeft.y) m_BB.BottomLeft.y = v.y;
				if (v.x > m_BB.TopRight.x) m_BB.TopRight.x = v.x;
				if (v.y > m_BB.TopRight.y) m_BB.TopRight.y = v.y;
			}
			break;

		case ColliderType2D::NONE:
		default:
			EIS_CORE_ERROR("Invalid Rigidbody2D type!");
		}
	}


	void Rigidbody2D::Draw(const glm::vec4& color, bool line) const
	{
		switch (m_Properties.Type)
		{
		case ColliderType2D::CIRCLE:
			Renderer2D::DrawCircle(m_Position, glm::vec2(m_Properties.Radius * 2.0f), color);
			if (line)
				Renderer2D::DrawLine(m_Position, glm::degrees(m_Rotation), m_Properties.Radius, glm::vec4(0, 0, 0, 1)); // Rotation visualiser
			break;

		case ColliderType2D::POLYGON:
			Renderer2D::DrawRotatedQuad(m_Position, m_Properties.Size, glm::degrees(m_Rotation), color);
			break;

		case ColliderType2D::NONE:
		default:
			EIS_CORE_ERROR("Invalid Rigidbody2D type!");
			break;
		}
	}

	void Rigidbody2D::Draw(Ref<Texture2D>& tex) const
	{
		switch (m_Properties.Type)
		{
		case ColliderType2D::CIRCLE:
			EIS_CORE_ERROR("Cannot draw textured circles!");
			break;

		case ColliderType2D::POLYGON:
			Renderer2D::DrawRotatedQuad(m_Position, m_Properties.Size, glm::degrees(m_Rotation), tex);
			break;

		case ColliderType2D::NONE:
		default:
			EIS_CORE_ERROR("Invalid Rigidbody2D type!");
			break;
		}
	}

	void Rigidbody2D::DrawBoundingBox() const
	{
		CalculateBoundingBox();
		Eis::Renderer2D::DrawLine(m_BB.BottomLeft, { m_BB.TopRight.x, m_BB.BottomLeft.y }, glm::vec4(0, 1, 0, 1));
		Eis::Renderer2D::DrawLine({ m_BB.TopRight.x, m_BB.BottomLeft.y }, m_BB.TopRight, glm::vec4(0, 1, 0, 1));
		Eis::Renderer2D::DrawLine(m_BB.TopRight, { m_BB.BottomLeft.x, m_BB.TopRight.y }, glm::vec4(0, 1, 0, 1));
		Eis::Renderer2D::DrawLine({ m_BB.BottomLeft.x, m_BB.TopRight.y }, m_BB.BottomLeft, glm::vec4(0, 1, 0, 1));
	}

	void Rigidbody2D::DrawVertices() const
	{
		if (m_Properties.Type == ColliderType2D::CIRCLE) return;

		CalculateTransformedVertices();
		Eis::Renderer2D::DrawCircle(m_TransformedVertices[0], glm::vec2(0.1f), glm::vec4(1.0f));
		Eis::Renderer2D::DrawCircle(m_TransformedVertices[1], glm::vec2(0.1f), glm::vec4(1.0f));
		Eis::Renderer2D::DrawCircle(m_TransformedVertices[2], glm::vec2(0.1f), glm::vec4(1.0f));
		Eis::Renderer2D::DrawCircle(m_TransformedVertices[3], glm::vec2(0.1f), glm::vec4(1.0f));
	}
}