#pragma once

#include <xhash>

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
	template<>
	struct hash<Mirage::UUID>
	{
		size_t operator()(const Mirage::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
