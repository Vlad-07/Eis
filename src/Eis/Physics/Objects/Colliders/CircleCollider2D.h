#pragma once

#include "Eis/Physics/Objects/Colliders/Collider2D.h"


namespace Eis
{
	class CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D(float radius);
		virtual ~CircleCollider2D() = default;

		float GetRadius() const { return m_Radius; }

	protected:
		virtual void CalcBBox(glm::vec2 pos, float rot) const override;

	private:
		float m_Radius;
	};
}