#pragma once

#include <vector>
#include "Eis/Physics/Objects/Rigidbody2D.h"


namespace Eis
{
	class PhysicsManager2D
	{
	public:
		PhysicsManager2D();
		PhysicsManager2D(const PhysicsManager2D&) = delete;
		PhysicsManager2D& operator=(const PhysicsManager2D&) = delete;

		static void Update(uint8_t iterations) { s_Instance->UpdateInternal(iterations); }


		// Rigidbodies must NOT be realicated!!!
		static void RegisterBody(Scope<Rigidbody2D>& rb) { EIS_ASSERT(rb, "Invalid rb!"); s_Instance->m_Bodies.emplace_back(*rb); }


		static Rigidbody2D& GetBody(size_t id);
		static auto& GetBodies() { return s_Instance->m_Bodies; }
		static size_t GetBodyCount() { return s_Instance->m_Bodies.size(); }
		static void ClearBodies() { s_Instance->m_Bodies.clear(); }

	private:
		void UpdateInternal(uint8_t iterations);
		void UpdateBodies(float timeScale);
		void BroadPhase();
		void NarrowPhase();

	private:
		static Scope<PhysicsManager2D> s_Instance;

		std::vector<std::reference_wrapper<Rigidbody2D>> m_Bodies;
		std::vector<std::pair<uint32_t, uint32_t>> m_Contacts;

	private:
		static constexpr glm::vec2 c_Gravity = glm::vec2(0.0f, -9.81f);
		static constexpr uint8_t c_MaxIterations = 64;
	};
}