#pragma once

#include <random>
#include <glm/glm.hpp>
#include "Eis/Debug/Instrumentor.h"


namespace Eis
{
	class Random
	{
	public:
		static void Init()
		{
			EIS_PROFILE_FUNCTION();

			s_RandomEngine.seed(std::random_device()());
		}

		static void SetSeed(uint32_t seed)
		{
			s_RandomEngine.seed(seed);
		}

		static bool Bool()
		{
			EIS_PROFILE_FUNCTION();

			s_BoolBits >>= 1;
			if (s_BoolBits == 1)
				s_BoolBits = s_Distribution(s_RandomEngine) | s_SentinelBit;

			return s_BoolBits & 1;
		}

		static uint32_t UInt()
		{
			EIS_PROFILE_FUNCTION();

			return s_Distribution(s_RandomEngine);
		}

		static uint32_t UInt(uint32_t min, uint32_t max)
		{
			EIS_PROFILE_FUNCTION();

			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static float Float()
		{
			EIS_PROFILE_FUNCTION();

			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static float Float(float min, float max)
		{
			EIS_PROFILE_FUNCTION();

			return min + ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max()) * (max - min);
		}

		static glm::vec2 Vec2()
		{
			EIS_PROFILE_FUNCTION();

			return glm::vec2(Float(), Float());
		}

		static glm::vec2 Vec2(float min, float max)
		{
			EIS_PROFILE_FUNCTION();

			return glm::vec2(Float(min, max), Float(min, max));
		}

		static glm::vec3 Vec3()
		{
			EIS_PROFILE_FUNCTION();

			return glm::vec3(Float(), Float(), Float());
		}

		static glm::vec3 Vec3(float min, float max)
		{
			EIS_PROFILE_FUNCTION();

			return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

		inline static uint32_t s_BoolBits = 2;
		static constexpr uint32_t s_SentinelBit = (1 << (sizeof(s_BoolBits) * 8 - 1));
	};
}