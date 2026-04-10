#pragma once

#include <memory>
#include <utility>
#include <functional>

#include <vector>
#include <array>
#include <optional>
#include <variant>
#include <string>
#include <sstream>
#include <ostream>
#include <unordered_map>
#include <unordered_set>

#include "Eis/Core/Log.h"
#include "Eis/Debug/Assert.h"
#include "Eis/Debug/Instrumentor.h"

#ifdef EIS_PLATFORM_WINDOWS
	#include <Windows.h>
#endif