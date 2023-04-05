#pragma once

namespace Mirage
{
    class FileDialogs
    {
    public:
        // Return empty string if canceled
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };

	class TimeUtils
	{
	public:
		static float GetTime();
	};
}
