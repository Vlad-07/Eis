#include "Eispch.h"
#include "UUID.h"

#include "Eis/Core/Random.h"


namespace Eis
{
	UUID::UUID()
		: m_UUID{ Random::UInt64() }
	{}
}