#include "endpoint.hpp"

#include <string>
#include <fstream>

namespace blog{
	// CEndpointSettings
	CEndpointSettings::CEndpointSettings(std::ostream& par_outStream, bool par_isEnabled) : pOutStream(&par_outStream), isEnabled(par_isEnabled){}
	CEndpointSettings::CEndpointSettings(bool par_isEnabled) : isEnabled(par_isEnabled), pOutStream(nullptr){}
	CEndpointSettings::~CEndpointSettings() = default;
	void CEndpointSettings::write(const std::string& str){
		if (this->isEnabled){
			(*(this->pOutStream)) << str << std::endl;
		}
	}
	// COwnEndpointSettings
	COwnEndpointSettings::COwnEndpointSettings(const std::string& endpointName, bool par_isEnabled) : CEndpointSettings(par_isEnabled){
		this->pOutStream = new std::ofstream(endpointName, std::ofstream::app);
	}

	COwnEndpointSettings::~COwnEndpointSettings(){
		if (this->pOutStream){
			((std::ofstream*)(this->pOutStream))->close();
			delete this->pOutStream;
		}
	}
};