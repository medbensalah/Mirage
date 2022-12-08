#pragma once

#include "Mirage/Core/UUID.h"

namespace Mirage
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};
}
