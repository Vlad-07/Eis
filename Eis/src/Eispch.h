#pragma once


#include "Eis/Core/PlatformDetection.h"

#ifdef EIS_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif


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
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Eis/Core/Core.h"
#include "Eis/Core/Log.h"
#include "Eis/Debug/Assert.h"
#include "Eis/Debug/Instrumentor.h"