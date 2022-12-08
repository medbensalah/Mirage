#include "MrgPch.h"

#include "UUID.h"

#include <random>

namespace Mirage
{
	static std::random_device sRandomDevice;
	static std::mt19937_64 sRandomEngine(sRandomDevice());
	static std::uniform_int_distribution<uint64_t> sRandomDistribution;
	
	UUID::UUID()
		: UUID(sRandomDistribution(sRandomEngine))
	{
		
	}

	UUID::UUID(uint64_t id)
		: m_UUID(id)
	{
		
	}
}
