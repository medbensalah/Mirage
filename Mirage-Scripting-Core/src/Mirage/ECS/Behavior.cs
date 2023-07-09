using System;
using System.Collections;
using System.Runtime.CompilerServices;

namespace Mirage
{
	public abstract class Behavior
	{
		protected Behavior()
		{
			Console.WriteLine("Behavior created with no uuid");
			_uuid = 0;
		}

		internal Behavior(ulong uuid)
		{
			Console.WriteLine("Behavior created with uuid {0}", uuid);
			_uuid = uuid;
		}

		public readonly ulong _uuid;
		//
		// protected abstract void OnCreate();
		// protected abstract void OnUpdate(float deltaTime);
		// protected abstract void OnPhysicsUpdate(float pDeltaTime);

		
		private Transform _transform = null;
		public Transform transform
		{
			get { return _transform ?? (_transform = GetComponent<Transform>()); }
			set { _transform = value; }
		}

		public bool HasComponent<T>() where T : Component, new()
		{
			Type type = typeof(T);
			return Internals.SO_HasComponent(_uuid, type);
		}
		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
			{
				return null;
			}

			Console.WriteLine("getting component");
			T component =  new T() { Behavior = this };
			return component;
		}
	}
}
