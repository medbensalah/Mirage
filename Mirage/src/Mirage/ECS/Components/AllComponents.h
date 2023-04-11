﻿#pragma once

namespace Mirage
{
	struct TransformComponent;
	struct SpriteRendererComponent;
	struct CircleRendererComponent;
	struct CameraComponent;
	struct NativeScriptComponent;
	struct RigidBody2DComponent;
	struct BoxCollider2DComponent;
	struct CircleCollider2DComponent;
	struct ScriptComponent;

	template <typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
	ComponentGroup<TransformComponent,
				   SpriteRendererComponent, CircleRendererComponent, CameraComponent,
				   ScriptComponent, NativeScriptComponent,
	               RigidBody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;
}
