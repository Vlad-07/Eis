#pragma once

#include <vector>

#include "Eis/Physics/Objects/Colliders/Collider2D.h"


namespace Eis
{
	class PolygonCollider2D : public Collider2D
	{
	public:
		PolygonCollider2D(glm::vec2 size);
		PolygonCollider2D(const std::vector<glm::vec2>& vertices);
		virtual ~PolygonCollider2D() = default;

		const std::vector<glm::vec2>& GetTransformedVertices(glm::vec2 pos, float rot) const;

	protected:
		void CalcTransformedVertices(glm::vec2 pos, float rot) const;
		virtual void CalcBBox(glm::vec2 pos, float rot) const override;

	private:
		std::vector<glm::vec2> m_Vertices;
		mutable std::vector<glm::vec2> m_TransformedVertices;
	};
}