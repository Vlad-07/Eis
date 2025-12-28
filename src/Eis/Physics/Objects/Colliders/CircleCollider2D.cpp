#include "Eispch.h"
#include "CircleCollider2D.h"

#include "Eis/Physics/Objects/Rigidbody2D.h"


namespace Eis
{
	CircleCollider2D::CircleCollider2D(float radius)
		: Collider2D(Type::CIRCLE), m_Radius(radius)
	{}


	void CircleCollider2D::CalcBBox(glm::vec2 pos, float rot) const
	{
		m_BBox = { pos + glm::vec2(-m_Radius), pos + glm::vec2(m_Radius)};
	}
}