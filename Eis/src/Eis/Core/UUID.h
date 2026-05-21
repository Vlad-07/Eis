#pragma once


namespace Eis
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid) : m_UUID{ uuid } {}
		~UUID() = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}


namespace std
{
	template<>
	struct hash<Eis::UUID>
	{
		std::size_t operator()(const Eis::UUID& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};
}