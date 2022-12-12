#pragma once

namespace Mirage
{
	class SettingsSerializer
	{
	public:
		void Serialize(const std::filesystem::path& path);
		void Deserialize(const std::filesystem::path& path);
	};
}
