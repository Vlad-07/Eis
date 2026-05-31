#include "Eispch.h"
#include "FileUtils.h"

#include <rapidhash/rapidhash.h>


namespace Eis::Utils
{
	std::string ReadFile(const std::filesystem::path& path)
	{
		std::ifstream in(path, std::ios::binary);

		if (!in)
		{
			EIS_CORE_ERROR("Could not open file: {}", path.string());
			return {};
		}

		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		in.seekg(0, std::ios::beg);

		if (size == -1)
		{
			EIS_CORE_ERROR("Could not read from file: {}", path.string());
			return {};
		}

		std::string result(size, '\0');
		in.read(result.data(), size);
		return result;
	}
}