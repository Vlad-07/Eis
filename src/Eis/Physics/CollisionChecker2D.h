#pragma once

#include "Objects/Rigidbody2D.h"


namespace Eis
{
	struct CollisionData2D
	{
		glm::vec2 Normal = glm::vec2();
		float Depth = std::numeric_limits<float>::max();
		float Sign = 1.0f;
	};


	class CollisionChecker2D
	{
	public:
		static bool CheckCollision(const Rigidbody2D& collider1, const Rigidbody2D& collider2, CollisionData2D& data)
		{
			if (collider1.GetType() == RigidbodyType2D::CIRCLE && collider2.GetType() == RigidbodyType2D::CIRCLE)
				return CollideCircles(collider1, collider2, data);
			else if (collider1.GetType() == RigidbodyType2D::BOX && collider2.GetType() == RigidbodyType2D::BOX)
				return CollidePolygons(collider1, collider2, data);
			else if (collider1.GetType() == RigidbodyType2D::CIRCLE && collider2.GetType() == RigidbodyType2D::BOX)
				return CollideCirclePolygon(collider1, collider2, data);
			else if (collider1.GetType() == RigidbodyType2D::BOX && collider2.GetType() == RigidbodyType2D::CIRCLE)
				return CollideCirclePolygon(collider2, collider1, data);
			return false;
		}


	private:
		static bool CollideCircles(const Rigidbody2D& collider1, const Rigidbody2D& collider2, CollisionData2D& data)
		{
			const float dist = glm::distance(collider1.GetPosition(), collider2.GetPosition());
			const float minDist = collider1.GetRadius() + collider2.GetRadius();

			if (dist >= minDist)
				return false;

			data.Normal = glm::normalize(collider2.GetPosition() - collider1.GetPosition());
			data.Depth = minDist - dist;
			data.Sign = 1.0f;

			return true;
		}



		static bool CollidePolygons(const Rigidbody2D& collider1, const Rigidbody2D& collider2, CollisionData2D& data)
		{
			data.Depth = std::numeric_limits<float>::max();
			data.Normal = glm::vec2();
			data.Sign = 1.0f;

			const ColliderVertices2D& vertices1 = collider1.GetTransformedVertices();
			const ColliderVertices2D& vertices2 = collider2.GetTransformedVertices();
			for (uint8_t i = 0; i < vertices1.length(); i++)
			{
				const glm::vec2& v1 = vertices1[i];
				const glm::vec2& v2 = vertices1[(i + 1) % vertices1.length()];

				const glm::vec2 edge = v2 - v1;

				const glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

				const ProjectionResult r1 = ProjectVertices(vertices1, axis);
				const ProjectionResult r2 = ProjectVertices(vertices2, axis);

				if (r1.Min >= r2.Max || r2.Min >= r1.Max)
					return false;

				float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);

				/*float depth = r1.Max - r2.Min;
				float sign = 1.0f;
				if (r2.Max - r1.Min < depth)
				{
					depth = r2.Max - r1.Min;
					sign = -1.0f;
				}*/

				if (depth < data.Depth)
				{
					data.Depth = depth;
					data.Normal = axis;
					data.Sign = 1.0f;
				}
			}

			for (uint8_t i = 0; i < vertices2.length(); i++)
			{
				const glm::vec2& v1 = vertices2[i];
				const glm::vec2& v2 = vertices2[(i + 1) % vertices2.length()];

				const glm::vec2 edge = v2 - v1;

				const glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

				const ProjectionResult r1 = ProjectVertices(vertices2, axis);
				const ProjectionResult r2 = ProjectVertices(vertices1, axis);

				if (r1.Min >= r2.Max || r2.Min >= r1.Max)
					return false;

				float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);

				/*float depth = r1.Max - r2.Min;
				float sign = -1.0f;
				if (r2.Max - r1.Min < depth)
				{
					depth = r2.Max - r1.Min;
					sign = 1.0f;
				}*/

				if (depth < data.Depth)
				{
					data.Depth = depth;
					data.Normal = axis;
					data.Sign = 1.0f;
				}
			}

			const glm::vec2 dir = collider2.GetPosition() - collider1.GetPosition();
			if (glm::dot(dir, data.Normal) < 0.0f)
				data.Normal = -data.Normal;

			return true;
		}



		static bool CollideCirclePolygon(const Rigidbody2D& circle, const Rigidbody2D& polygon, CollisionData2D& data)
		{
			data.Depth = std::numeric_limits<float>::max();
			data.Normal = glm::vec2();
			data.Sign = 1.0f;

			const ColliderVertices2D& vertices = polygon.GetTransformedVertices();
			for (uint8_t i = 0; i < vertices.length(); i++)
			{
				const glm::vec2& v1 = vertices[i];
				const glm::vec2& v2 = vertices[(i + 1) % vertices.length()];

				const glm::vec2 edge = v2 - v1;

				const glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

				const ProjectionResult r1 = ProjectVertices(vertices, axis);
				const ProjectionResult r2 = ProjectCircle(circle.GetPosition(), circle.GetRadius(), axis);

				if (r1.Min >= r2.Max || r2.Min >= r1.Max)
					return false;

				float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);

				/*float depth = r1.Max - r2.Min;
				float sign = 1.0f;
				if (r2.Max - r1.Min < depth)
				{
					depth = r2.Max - r1.Min;
					sign = -1.0f;
				}*/

				if (depth < data.Depth)
				{
					data.Depth = depth;
					data.Normal = axis;
					data.Sign = 1.0f;
				}
			}

			const int cpIndex = ClosestVertexToPoint(circle.GetPosition(), vertices);
			const glm::vec2 cp = vertices[cpIndex];

			const glm::vec2 axis = glm::normalize(cp - circle.GetPosition());

			const ProjectionResult r1 = ProjectVertices(vertices, axis);
			const ProjectionResult r2 = ProjectCircle(circle.GetPosition(), circle.GetRadius(), axis);

			if (r1.Min >= r2.Max || r2.Min >= r1.Max)
				return false;

			float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);

			/*float depth = r1.Max - r2.Min;
			float sign = 1.0f;
			if (r2.Max - r1.Min < depth)
			{
				depth = r2.Max - r1.Min;
				sign = -1.0f;
			}*/

			if (depth < data.Depth)
			{
				data.Depth = depth;
				data.Normal = axis;
				data.Sign = 1.0f;
			}

			const glm::vec2 dir = polygon.GetPosition() - circle.GetPosition();
			if (glm::dot(dir, data.Normal) < 0.0f)
				data.Normal = -data.Normal;

			return true;
		}




	private:
		struct ProjectionResult
		{
			float Min = std::numeric_limits<float>::max();
			float Max = std::numeric_limits<float>::min();
		};

		static ProjectionResult ProjectVertices(const ColliderVertices2D& vertices, glm::vec2 axis)
		{
			ProjectionResult result;

			for (uint8_t i = 0; i < vertices.length(); i++)
			{
				const glm::vec2& vertex = vertices[i];
				const float proj = glm::dot(vertex, axis);

				if (proj < result.Min) result.Min = proj;
				if (proj > result.Max) result.Max = proj;
			}

			return result;
		}

		static ProjectionResult ProjectCircle(glm::vec2 center, float radius, glm::vec2 axis)
		{
			const glm::vec2 radiusOnAxis = axis * radius;

			const glm::vec2 p1 = center + radiusOnAxis;
			const glm::vec2 p2 = center - radiusOnAxis;

			ProjectionResult result{ glm::dot(p1, axis), glm::dot(p2, axis) };

			if (result.Min > result.Max)
				std::swap(result.Min, result.Max);

			return result;
		}

		static uint8_t ClosestVertexToPoint(glm::vec2 point, const ColliderVertices2D& vertices)
		{
			float minDist = std::numeric_limits<float>::max();
			int16_t index = -1;

			for (uint8_t i = 0; i < vertices.length(); i++)
			{
				const float dist = glm::distance(point, vertices[i]);

				if (dist < minDist)
				{
					minDist = dist;
					index = i;
				}
			}

			return index;
		}
	};
}