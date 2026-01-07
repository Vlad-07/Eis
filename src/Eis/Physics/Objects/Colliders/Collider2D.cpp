#include "Eispch.h"
#include "Collider2D.h"

#include "Eis/Physics/Objects/Rigidbody2D.h"
#include "Eis/Physics/Objects/Colliders/CircleCollider2D.h"
#include "Eis/Physics/Objects/Colliders/PolygonCollider2D.h"


namespace Eis
{
    const BBox2D& Collider2D::GetBBox(glm::vec2 pos, float rot) const
    {
        if (m_UpdateBBox)
        {
            CalcBBox(pos, rot);
            m_UpdateBBox = false;
        }

        return m_BBox;
    }


    Scope<Collider2D> Collider2D::Create(float radius)
    {
        return CreateScope<CircleCollider2D>(radius);
    }
    Scope<Collider2D> Collider2D::Create(glm::vec2 size)
    {
        return CreateScope<PolygonCollider2D>(size);
    }
    Scope<Collider2D> Collider2D::Create(const Vertices& vertices)
    {
        return CreateScope<PolygonCollider2D>(vertices);
    }
}