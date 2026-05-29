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
			s_RandomEngine.seed(s_RandDevice());
		}

		static void SetSeed(uint64_t seed)
		{
			s_RandomEngine.seed(seed);
		}


		static bool Bool()
		{
			s_BoolBits >>= 1;
			if (s_BoolBits == 1)
				s_BoolBits = s_Distribution(s_RandomEngine) | s_SentinelBit;

			return s_BoolBits & 1;
		}

		static uint32_t UInt32()
		{
			// top bits are supposed to be marginally better?
			return static_cast<uint32_t>(s_Distribution(s_RandomEngine) >> 32);
		}

		static uint32_t UInt32(uint32_t min, uint32_t max)
		{
			return min + (UInt32() % (max - min + 1));
		}

		static uint64_t UInt64()
		{
			return s_Distribution(s_RandomEngine);
		}

		static uint64_t UInt64(uint64_t min, uint64_t max)
		{
			return min + (UInt64() % (max - min + 1));
		}

		// [0, 1)
		static float Float()
		{
			// is this good?
			return (UInt32() >> 8) * (1.0f / (1 << 24));
		}

		static float Float(float min, float max)
		{
			return min + Float() * (max - min);
		}

		// [0, 1)
		static glm::vec2 Vec2()
		{
			return glm::vec2(Float(), Float());
		}

		static glm::vec2 Vec2(float min, float max)
		{
			return glm::vec2(Float(min, max), Float(min, max));
		}

		// [0, 1)
		static glm::vec3 Vec3()
		{
			return glm::vec3(Float(), Float(), Float());
		}

		static glm::vec3 Vec3(float min, float max)
		{
			return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
		}

		// [0, 1)
		static glm::vec4 Vec4()
		{
			return glm::vec4(Float(), Float(), Float(), Float());
		}

		static glm::vec4 Vec4(float min, float max)
		{
			return glm::vec4(Float(min, max), Float(min, max), Float(min, max), Float(min, max));
		}

	private:
		using EngineType = std::mt19937_64;
		using ResultType = EngineType::result_type;


		static inline std::random_device s_RandDevice;
		static inline EngineType s_RandomEngine;
		static inline std::uniform_int_distribution<EngineType::result_type> s_Distribution;

		inline static ResultType s_BoolBits = 2;
		static constexpr ResultType s_SentinelBit = (1ull << (sizeof(s_BoolBits) * 8 - 1));
	};
}