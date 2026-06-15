#pragma once

#include "Eis/Core/Core.h"

#include <filesystem>


namespace Eis::Utils
{
	std::string ReadFile(const std::filesystem::path& path);
}