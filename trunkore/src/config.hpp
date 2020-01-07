#ifndef HEADER_CONFIG
#define HEADER_CONFIG

#include <string>

//include common platform-specific headers
#if defined PLATFORM_WINDOWS
#include <windows.h>
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

	using TYPE_JOBID = std::wstring;
	using TYPE_JOBMODID = platform::TYPE_FILESYSTEMPATH;
	using TYPE_JOBCONFIGID = platform::TYPE_FILESYSTEMPATH;

	/*
	//WARNING Include loop config.hpp-i_modulecontainer.hpp-<platform>modulecontainer.hpp
	#if defined PLATFORM_WINDOWS
		using TYPE_MODULECONTAINER = CWinModuleContainer;
	#elif defined PLATFORM_NIX
		//TODO
	#endif
	*/

	const platform::TYPE_FUNCNAME getJobFunctionName = "getJob";
}

#endif
