#include "log.hpp"

#include <string>
#include <map>

namespace blog{
	// Endpoints
	std::map<std::string, CEndpointSettings*> blogEndpoints;

	void addEndpoint(std::ostream& outStream, const std::string& endpointName, bool isEnabled){
		if (blogEndpoints.find(endpointName) != blogEndpoints.cend()){
			throw ExDuplicateEndpointNames();
		}
		auto pEndpointSettings = new CEndpointSettings(outStream, isEnabled);
		blogEndpoints.insert({endpointName, pEndpointSettings});
	}
	
	void addEndpoint(const std::string& fileName, const std::string& endpointName, bool isEnabled){
		if (blogEndpoints.find(endpointName) != blogEndpoints.cend()){
			throw ExDuplicateEndpointNames();
		}
		auto pEndpointSettings = new COwnEndpointSettings(fileName, isEnabled);
		blogEndpoints.insert({ endpointName, pEndpointSettings });
	}
	
	void removeEndpoint(const std::string& endpointName){
		auto endpointIter = blogEndpoints.find(endpointName);
		if (endpointIter==blogEndpoints.cend()){
			throw ExEndpointNotFound();
		}
		delete endpointIter->second;
		blogEndpoints.erase(endpointIter);
	}
	
	// Loggers
	std::map<std::string/*logger name*/, CLogger*/*logger ptr*/> globalLoggers;

	CLogger* createGlobalLogger(const std::string& globalLoggerName){
		auto pNewLogger = new CLogger{};
		globalLoggers.insert({globalLoggerName, pNewLogger});
		return pNewLogger;
	}

	CLogger* getGlobalLogger(const std::string& globalLoggerName){
		auto globalLoggersIter = globalLoggers.find(globalLoggerName);
		if (globalLoggersIter == globalLoggers.cend()){
			throw ExLoggerNotFound();
		}
		return globalLoggersIter->second;
	}

	void removeGlobalLogger(const std::string& globalLoggerName){
		auto globalLoggersIter = globalLoggers.find(globalLoggerName);
		if (globalLoggersIter == globalLoggers.cend()){
			throw ExLoggerNotFound();
		}
		delete globalLoggersIter->second;
		globalLoggers.erase(globalLoggersIter);
	}
}