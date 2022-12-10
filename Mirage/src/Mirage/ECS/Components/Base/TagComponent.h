#pragma once

namespace Mirage
{
	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	
		std::string Tag;
	};
}
