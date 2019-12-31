#ifndef HEADER_I_MODULEMANAGER
#define HEADER_I_MODULEMANAGER

#include <string>

#include "config.hpp"

#include "utils.hpp"
#include "types.hpp"

//====================================================================================================
/**
	Particular module container class
*/
class IModuleManager {
public:
	using TYPE_FUNCNAME=config::platform::TYPE_FUNCNAME;
	using TYPE_JOBMODID = config::platform::TYPE_JOBMODID;

	virtual void loadModule(const TYPE_JOBMODID &) = 0;
	virtual void unloadModule() = 0;
	virtual TYPE_SIZE getFunctionAddress(const TYPE_FUNCNAME &)=0;
	virtual ~IModuleManager() {}
//Exceptions
	//Module manager common exception class to handle them in platform independent manner
	class ExModuleManager : public ExEx {};
};

//====================================================================================================
/**
	Particular module container class using RAII-concept
*/
class IModuleManagerRAII {
public:
	using TYPE_FUNCNAME = config::platform::TYPE_FUNCNAME;

	virtual TYPE_SIZE getFunctionAddress(const TYPE_FUNCNAME &) = 0;
	virtual ~IModuleManagerRAII() {}
//Exceptions
	class ExModuleManagerRAII : public ExEx {};
};

//TODO Think about common module manager exception which will be used by both RAII and non-RAII versions
class ExModuleManager : public ExEx {};

#endif
