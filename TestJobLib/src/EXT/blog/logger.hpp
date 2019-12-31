#ifndef HEADER_LOGGER
#define HEADER_LOGGER

#include <set>
#include <string>

namespace blog{
	class CLogger{
		std::set<std::string> endpoints;

	public:
		bool isEnabled;

		CLogger();
		virtual ~CLogger();
		void addEndpoint(const std::string&);
		void removeEndpoint(const std::string&);

		void log(const std::string& logStr);
	};
}

#endif