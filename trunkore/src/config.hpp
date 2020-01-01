#ifndef HEADER_CONFIG
#define HEADER_CONFIG

#include <string>

#include "types.hpp"

//include common platform-specific headers
#if defined PLATFORM_WINDOWS
	#include <windows.h>
	#include "ModuleContainer/winmodulecontainer.hpp"
#elif defined PLATFORM_NIX
//TODO
#endif

namespace config {
	namespace platform {
#if defined PLATFORM_WINDOWS
		using TYPE_FILESYSTEMPATH = LPCWSTR;
		using TYPE_FUNCNAME = LPCSTR;
#elif defined PLATFORM_NIX
		//TODO
#endif
	}

	using TYPE_JOBID = std::string;
	using TYPE_JOBMODID = platform::TYPE_FILESYSTEMPATH;

#if defined PLATFORM_WINDOWS
	using TYPE_MODULECONTAINER = CWinModuleContainer;
#elif defined PLATFORM_NIX
	//TODO
#endif

	extern std::string getJobFunctionName;
}

#endif