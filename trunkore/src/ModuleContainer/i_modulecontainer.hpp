#ifndef HEADER_I_MODULECONTAINER
#define HEADER_I_MODULECONTAINER

#include <string>

#include "utils.hpp"
#include "types.hpp"
#include "config.hpp"

//====================================================================================================
/**
	Particular module container class
*/

class IModuleContainer {
public:
	using TYPE_FUNCNAME = config::platform::TYPE_FUNCNAME;
	using TYPE_JOBMODID = config::TYPE_JOBMODID;

	virtual void loadModule(const TYPE_JOBMODID &) = 0;
	virtual void unloadModule() = 0;
	virtual TYPE_SIZE getFunctionAddress(const TYPE_FUNCNAME &) = 0;
	virtual ~IModuleContainer() {}
};

//====================================================================================================
/**
	Particular module container class using RAII-concept
*/
class IModuleContainerRAII {
public:
	using TYPE_FUNCNAME = config::platform::TYPE_FUNCNAME;

	virtual TYPE_SIZE getFunctionAddress(const TYPE_FUNCNAME &) = 0;
	virtual ~IModuleContainerRAII() {}
};

// Module container exception class
//====================================================================================================
class ExModuleContainer : public ExEx {};//WARNING ExModuleContainer not in scope of IModuleContainer/IModuleContainerRAII
//TODO Mb IModuleContainer/IModuleContainerRAII/ExModuleContainer in one namespace, and then "using IModuleContainer,using IModuleContainer" to keep exception class more "scoped"
#endif
