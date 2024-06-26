#pragma once

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <sdkddkver.h>
#include <windows.h>
#include <shlwapi.h>
#include <d3d11.h>
#include <intrin.h>

#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <cctype>
#include <inttypes.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <deque>
#include <stack>
#include <unordered_map>
#include <atomic>
#include <functional>
#include <thread>
#include <valarray>
#include <numeric>
#include <algorithm>

#include "CharStream.hpp"
#include "XorString.hpp"
#include "Branding.hpp"
#include "Enums.hpp"
#include "Logger.hpp"
#include "Types.hpp"

using namespace Vega::Natives;