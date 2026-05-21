#include "Eispch.h"
#include "UUID.h"

#include <random>


namespace Eis
{
	static std::random_device s_RandDevice;
	static std::mt19937_64 s_RandEngine{ s_RandDevice() };
	static std::uniform_int_distribution<uint64_t> s_Distrib;

	UUID::UUID()
		: m_UUID{ s_Distrib(s_RandEngine) }
	{}
}