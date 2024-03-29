#pragma once

#include <utility>
#include <functional>
#include <random>
#include <chrono>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Eis/Core/Log.h"
#include "Eis/Debug/Instrumentor.h"

#ifdef EIS_PLATFORM_WINDOWS
	#include <Windows.h>
#endif