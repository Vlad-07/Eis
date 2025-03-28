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
	};
}