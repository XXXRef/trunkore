#ifndef HEADER_ENDPOINT
#define HEADER_ENDPOINT

#include <ostream>
#include <string>
#include <utility>

namespace blog{
	class CEndpointSettings{
	protected:
		std::ostream* pOutStream;

	public:
		bool isEnabled;
		CEndpointSettings(bool par_isEnabled = true);
		CEndpointSettings(std::ostream& par_outStream, bool par_isEnabled = true);
		virtual ~CEndpointSettings();
		virtual void write(const std::string& /*str*/);
	};

	class COwnEndpointSettings : public CEndpointSettings{
	public:
		COwnEndpointSettings(const std::string& endpointName, bool par_isEnabled = true);
		~COwnEndpointSettings();
	};

	using Endpoint = std::pair<std::string, CEndpointSettings*>;
}

#endif