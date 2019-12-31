#ifndef HEADER_CONFIG
#define HEADER_CONFIG

#include <string>

#include "types.hpp"

//include common platform-specific headers
#if defined PLATFORM_WINDOWS
	#include <windows.h>
#elif defined PLATFORM_NIX
	//TODO
#endif

namespace config{
	namespace platform {
		#if defined PLATFORM_WINDOWS
			using TYPE_JOBMODID = LPCWSTR;
			using TYPE_FUNCNAME = LPCSTR;
		#elif defined PLATFORM_NIX
			//TODO
		#endif
	}

	using TYPE_JOBID=std::string;

	extern std::string getJobFunctionName;
}

#endif