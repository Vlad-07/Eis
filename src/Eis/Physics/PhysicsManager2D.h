#pragma once

#include <vector>
#include <Eis/Physics/Objects/Rigidbody2D.h>
#include <Eis/Physics/CollisionChecker2D.h>
#include <Eis/Physics/CollisionSolver2D.h>


namespace Eis
{
	class PhysicsManager2D
	{
	public:
		PhysicsManager2D();
		PhysicsManager2D(const PhysicsManager2D&) = delete;
		PhysicsManager2D& operator=(const PhysicsManager2D&) = delete;

		static PhysicsManager2D& Get() { return *s_Instance; }


		static void Update(uint8_t iterations, const Eis::TimeStep ts) { s_Instance->UpdateInternal(iterations, ts); }


		static void AddBody(const Rigidbody2D& body) { s_Instance->m_Bodies.push_back(body); }
		static void AddBody(glm::vec2 pos, float radius, float density, float restitution, bool isStatic = false);
		static void AddBody(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic = false);
		static void RemoveBody(uint32_t id) { s_Instance->m_Bodies.erase(s_Instance->m_Bodies.begin() + id); }


		static Rigidbody2D& GetBody(uint32_t id);
		static auto& GetBodies() { return s_Instance->m_Bodies; }
		static uint32_t GetBodyCount() { return s_Instance->m_Bodies.size(); }
		static void ClearBodies() { s_Instance->m_Bodies.clear(); }

	private:
		void UpdateInternal(uint8_t iterations, const Eis::TimeStep ts);
		void UpdateBodies(const Eis::TimeStep ts);
		void BroadPhase();
		void NarrowPhase();

	private:
		static Scope<PhysicsManager2D> s_Instance;

		std::vector<Rigidbody2D> m_Bodies;
		std::vector<std::pair<uint32_t, uint32_t>> m_Contacts;

	private:
		static constexpr glm::vec2 c_Gravity = glm::vec2(0.0f, -9.81f);
		static constexpr uint8_t c_MaxIterations = 64;
	};
}