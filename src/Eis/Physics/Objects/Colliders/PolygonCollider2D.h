#pragma once

#include <vector>

#include "Eis/Physics/Objects/Colliders/Collider2D.h"


namespace Eis
{
	class PolygonCollider2D : public Collider2D
	{
	public:
		PolygonCollider2D(glm::vec2 size);
		PolygonCollider2D(const Vertices& vertices);
		virtual ~PolygonCollider2D() = default;

		const Vertices& GetVertices() const { return m_Vertices; }
		const Vertices& GetTransformedVertices(glm::vec2 pos, float rot) const;

	protected:
		void CalcTransformedVertices(glm::vec2 pos, float rot) const;
		virtual void CalcBBox(glm::vec2 pos, float rot) const override;

	private:
		Vertices m_Vertices;
		mutable Vertices m_TransformedVertices;
	};
}