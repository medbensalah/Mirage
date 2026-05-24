#pragma once

#include "Mirage/ECS/Components/Base/TransformComponent.h"
#include "Mirage/ECS/Components/Physics/BoxCollider2DComponent.h"
#include "Mirage/ECS/Components/Physics/CircleCollider2DComponent.h"
#include "Mirage/ECS/Components/Physics/Rigidbody2DComponent.h"
#include "Mirage/ECS/Components/Rendering/CameraComponent.h"
#include "Mirage/ECS/Components/Rendering/CircleRendererComponent.h"
#include "Mirage/ECS/Components/Rendering/SpriteRendererComponent.h"
#include "Mirage/ECS/Components/NativeScriptComponent.h"
#include "Mirage/ECS/Components/ScriptComponent.h"

namespace Mirage
{
	template <typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
	ComponentGroup<TransformComponent,
				   SpriteRendererComponent, CircleRendererComponent, CameraComponent,
				   ScriptComponent, NativeScriptComponent,
	               Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;
}
