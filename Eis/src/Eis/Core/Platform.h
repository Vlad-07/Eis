#pragma once

#include <cstdint>


namespace Eis
{
	class Platform
	{
	public:
		enum class PlatformType
		{
			NONE = 0, WINDOWS, WEB
		};

		static PlatformType GetPlatform() { return s_Platform; }

	private:
		static PlatformType s_Platform;
	};
}