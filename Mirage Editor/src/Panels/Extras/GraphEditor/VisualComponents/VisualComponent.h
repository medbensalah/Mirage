#pragma once

namespace Mirage::VisualComponents
{
	class VisualComponent
	{
	public:
		VisualComponent() {}
		virtual ~VisualComponent() {}

		virtual void Draw(float scale) {}
	};
}
