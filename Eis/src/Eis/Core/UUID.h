#pragma once

#include <cstdint>


namespace Eis
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid) : m_UUID{ uuid } {}
		UUID(const UUID& uuid) : m_UUID{ uuid.m_UUID } {}
		~UUID() = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}


namespace std
{
	template<typename T> struct hash;

	template<>
	struct hash<Eis::UUID>
	{
		std::size_t operator()(const Eis::UUID& uuid) const
		{
			return static_cast<uint64_t>(uuid);
		}
	};
}