#pragma once

#include <glm/glm.hpp>

#include "Eis/Core/Core.h"


namespace Eis
{
	using Vertices = std::vector<glm::vec2>;

	// Axis aligned bounding box
	struct BBox2D
	{
		glm::vec2 BottomLeft;
		glm::vec2 TopRight;
	};

	class Collider2D
	{
	public:
		enum class Type
		{
			NONE = 0, CIRCLE, POLYGON
		};

	public:
		Collider2D(Type t) : m_Type(t) {}
		virtual ~Collider2D() = default;

		Type GetType() const { return m_Type; }

		template<typename T>
		inline const T& As() const
		{
			static_assert(std::is_base_of_v<Collider2D, T>); // no runtime check!
			return static_cast<const T&>(*this);
		}

		const BBox2D& GetBBox(glm::vec2 pos, float rot) const;

		// Deffered update
		void Update() { m_UpdateBBox = true; m_UpdateVerts = true; }


		static Scope<Collider2D> Create(float radius);
		static Scope<Collider2D> Create(glm::vec2 size);
		static Scope<Collider2D> Create(const Vertices& vertices);

	protected:
		virtual void CalcBBox(glm::vec2 pos, float rot) const = 0;

	protected:
		mutable BBox2D m_BBox{};

		mutable bool m_UpdateBBox = true;
		mutable bool m_UpdateVerts = true;

		const Type m_Type;
	};
}