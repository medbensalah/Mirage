#pragma once

namespace Mirage
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t id);
		UUID(const UUID& id) = default;

		operator uint64_t() const { return m_UUID; }
		
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template <typename T> struct hash;
	template<>
	struct hash<Mirage::UUID>
	{
		size_t operator()(const Mirage::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}
