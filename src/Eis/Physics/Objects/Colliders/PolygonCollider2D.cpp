#include "Eispch.h"
#include "PolygonCollider2D.h"

#include "Eis/Physics/Objects/Rigidbody2D.h"


namespace Eis
{
	PolygonCollider2D::PolygonCollider2D(glm::vec2 size)
		: Collider2D(Type::POLYGON), m_Vertices(4), m_TransformedVertices(4)
	{
		m_Vertices[0] = { -size.x / 2.0f, -size.y / 2.0f };
		m_Vertices[1] = { size.x / 2.0f, -size.y / 2.0f };
		m_Vertices[2] = { size.x / 2.0f,  size.y / 2.0f };
		m_Vertices[3] = { -size.x / 2.0f,  size.y / 2.0f };
	}

	PolygonCollider2D::PolygonCollider2D(const Vertices& vertices)
		: Collider2D(Type::POLYGON), m_Vertices(vertices), m_TransformedVertices(vertices.size())
	{}

	const Vertices& PolygonCollider2D::GetTransformedVertices(glm::vec2 pos, float rot) const
	{
		if (m_UpdateVerts)
		{
			CalcTransformedVertices(pos, rot);
			m_UpdateVerts = false;
		}
		return m_TransformedVertices;
	}


	void PolygonCollider2D::CalcTransformedVertices(glm::vec2 pos, float rad) const
	{
		const float sin = glm::sin(rad);
		const float cos = glm::cos(rad);
		for (uint8_t i = 0; i < m_Vertices.size(); i++)
		{
			const glm::vec2& vertex = m_Vertices[i];
			m_TransformedVertices[i] = { cos * vertex.x - sin * vertex.y + pos.x,
										 sin * vertex.x + cos * vertex.y + pos.y };
		}
	}

	void PolygonCollider2D::CalcBBox(glm::vec2 pos, float rot) const
	{
		CalcTransformedVertices(pos, rot);
		m_BBox = { glm::vec2(std::numeric_limits<float>::max()), glm::vec2(std::numeric_limits<float>::lowest()) };
		for (uint8_t i = 0; i < m_TransformedVertices.size(); i++)
		{
			const glm::vec2& v = m_TransformedVertices[i];

			if (v.x < m_BBox.BottomLeft.x) m_BBox.BottomLeft.x = v.x;
			if (v.y < m_BBox.BottomLeft.y) m_BBox.BottomLeft.y = v.y;
			if (v.x > m_BBox.TopRight.x)   m_BBox.TopRight.x = v.x;
			if (v.y > m_BBox.TopRight.y)   m_BBox.TopRight.y = v.y;
		}
	}
}