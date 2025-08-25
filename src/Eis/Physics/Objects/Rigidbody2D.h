#pragma once

#include <glm/glm.hpp>
#include <Eis/Rendering/Objects/Texture.h>
#include <Eis/Core/TimeStep.h>


namespace Eis
{
	enum class ColliderType2D : uint8_t
	{
		NONE = 0, CIRCLE, POLYGON
	};

	typedef glm::mat4x2 ColliderVertices2D;

	struct RigidbodyProperties2D
	{
		ColliderType2D Type;

		bool Static;
		float Mass;
		float InvMass;
		float Density;
		float Area;
		float Restitution;
		float AngularInertia;
		float InvAngularInertia;
		float StaticFriction;
		float DynamicFriction;

		float Radius;

		glm::vec2 Size;
		ColliderVertices2D Vertices;
	};

	struct AlignedBoundingBox2D
	{
		glm::vec2 BottomLeft;
		glm::vec2 TopRight;
	};


	class Rigidbody2D
	{
	public:
		Rigidbody2D() = delete;
		// Circle constructor
		Rigidbody2D(glm::vec2 pos, float radius, float density, float restitution, bool isStatic = false);
		// Box constructor
		Rigidbody2D(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic = false);
		virtual ~Rigidbody2D() = default;


		void Update(const Eis::TimeStep& ts, const glm::vec2 gravity);


		void Move(glm::vec2 pos) { m_Position += pos; m_UpdateTrVert = true; m_UpdateBB = true; }
		void MoveTo(glm::vec2 pos) { m_Position = pos; m_UpdateTrVert = true; m_UpdateBB = true; }

		void Rotate(float deg) { m_Rotation += glm::radians(deg); m_UpdateTrVert = true; m_UpdateBB = true; }
		void RotateTo(float deg) { m_Rotation = glm::radians(deg); m_UpdateTrVert = true; m_UpdateBB = true; }

		void RotateRad(float rad) { m_Rotation += rad; m_UpdateTrVert = true; m_UpdateBB = true; }
		void RotateToRad(float rad) { m_Rotation = rad; m_UpdateTrVert = true; m_UpdateBB = true; }


		// Force in N
		void AddForce(const glm::vec2 force) { m_Force += force; }
		// Acceleration in m/s^2
		void AddAcceleration(const glm::vec2 accel) { m_Acceleration += accel; }
		// Linear velocity in m/s
		void AddLinearVel(const glm::vec2 linearVel) { m_LinearVelAdd += linearVel; }
		// Angular velocity in rad/s
		void AddAngularVel(float angVel) { m_AngVelAdd += angVel; }

		glm::vec2 GetPosition() const { return m_Position; }
		// Linear velocity in m/s
		glm::vec2 GetLinearVelocity() const { return m_LinearVelocity; }
		// Angular velocity in rad/s
		float GetAngularVelocity() const { return m_AngularVelocity; }

		const RigidbodyProperties2D& GetProperties() const { return m_Properties; }
		ColliderType2D GetType() const { return m_Properties.Type; }
		glm::vec2 GetSize() const { return m_Properties.Size; }
		float GetRadius() const { return m_Properties.Radius; }
		bool GetStatic() const { return m_Properties.Static; }
		float GetMass() const { return m_Properties.Mass; }
		float GetInvMass() const { return m_Properties.InvMass; }
		float GetInvAngInertia() const { return m_Properties.InvAngularInertia; }

		const ColliderVertices2D& GetTransformedVertices() const;
		const AlignedBoundingBox2D& GetBoundingBox() const;


		void Draw(const glm::vec4& color) const;
		void Draw(Ref<Texture2D>& tex) const;

		void DrawBoundingBox() const;
		void DrawVertices() const;

	private:
		void CalculateTransformedVertices() const;
		void CalculateBoundingBox() const;

	private:
		RigidbodyProperties2D m_Properties;

		glm::vec2 m_Position = glm::vec2(); // m
		float m_Rotation = 0.0f; // rad

		glm::vec2 m_LinearVelocity = glm::vec2(0.0f); // m/s
		float m_AngularVelocity = 0.0f; // rad/s

		glm::vec2 m_Force = glm::vec2(); // N (kg * m / s^2)
		glm::vec2 m_Acceleration = glm::vec2(); // m/s^2
		glm::vec2 m_LinearVelAdd = glm::vec2(); // m/s

		float m_AngVelAdd = 0.0f;

		mutable ColliderVertices2D m_TransformedVertices = ColliderVertices2D();

		mutable AlignedBoundingBox2D m_BB{};
		mutable bool m_UpdateTrVert = true;
		mutable bool m_UpdateBB = true;
	};
}