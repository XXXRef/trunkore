#include "logger.hpp"

#include "log.hpp"
#include "endpoint.hpp"

namespace blog{
	CLogger::CLogger(): isEnabled(true){}

	CLogger::~CLogger(){}
	
	void CLogger::addEndpoint(const std::string& endpointName){
		this->endpoints.insert(endpointName);
	}

	void CLogger::removeEndpoint(const std::string& endpointName){
		this->endpoints.erase(endpointName);
	}

	void CLogger::log(const std::string& logStr){
		if (this->isEnabled){
			for (auto i = this->endpoints.begin(); i != this->endpoints.end(); ++i){
				auto curEndpointSettings = blogEndpoints.find(*i);
				if (curEndpointSettings==blogEndpoints.end()){
					throw ExEndpointNotFound();
				}
				curEndpointSettings->second->write(logStr);
			}
		}
	}
}