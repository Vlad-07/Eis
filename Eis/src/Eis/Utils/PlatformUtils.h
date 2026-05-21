#pragma once

#include <filesystem>


namespace Eis
{
	class FileDialogs
	{
	public:
		static std::filesystem::path OpenFile(const char* filters);
		static std::filesystem::path SaveFile(const char* filters);
	};
}