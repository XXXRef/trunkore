#ifndef HEADER_LOG
#define HEADER_LOG

#include <exception>
#include <map>
#include <fstream>

#include "logger.hpp"
#include "endpoint.hpp"

namespace blog{
	// Endpoints
	extern std::map<std::string, CEndpointSettings*> blogEndpoints;
	void addEndpoint(std::ostream& outStream, const std::string& endpointName, bool isEnabled=true);
	void addEndpoint(const std::string& fileName, const std::string& endpointName, bool isEnabled = true);
	void removeEndpoint(const std::string& endpointName);
	// Loggers
	extern class CLogger;
	extern std::map<std::string/*logger name*/, CLogger* /*logger ptr*/> globalLoggers;
	CLogger* createGlobalLogger(const std::string& globalLoggerName);
	CLogger* getGlobalLogger(const std::string& globalLoggerName);
	void removeGlobalLogger(const std::string& globalLoggerName);
	// Exceptions
	class ExDuplicateEndpointNames : public std::exception{};
	class ExEndpointNotFound : public std::exception{};
	class ExLoggerNotFound : public std::exception{};
}

#endif