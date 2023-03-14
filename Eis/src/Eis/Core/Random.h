#pragma once

#include "Eispch.h"
#include <glm/glm.hpp>

// Stupid max() macro in "Windows.h"
#pragma push_macro("max")
#undef max

namespace Eis
{
	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static void SetSeed(uint32_t seed)
		{
			s_RandomEngine.seed(seed);
		}

		static uint32_t UInt()
		{
			return s_Distribution(s_RandomEngine);
		}

		static uint32_t UInt(uint32_t min, uint32_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static float Float(float min, float max)
		{
			return min + ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max()) * (max - min);
		}

		static glm::vec3 Vec3()
		{
			return glm::vec3(Float(), Float(), Float());
		}

		static glm::vec3 Vec3(float min, float max)
		{
			return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
}

#pragma pop_macro("max")