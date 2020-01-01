#ifndef HEADER_MODULESMANAGER
#define HEADER_MODULESMANAGER

#include <map>
#include <utility>
#include <string>
#include <memory>
#include <exception>

#include "types.hpp"
#include "utils.hpp"
#include "ModuleContainer/i_modulecontainer.hpp"

#include "config.hpp"

//====================================================================================================
/**
	\brief Handles job modules

	ModulesContainer class is intentionally platform-independent. All platform-specific business is done in appropriate ModuleManager classes
*/
class CModulesManager {
public: // public types
	using TYPE_JOBMODID = config::TYPE_JOBMODID; //just not to write "config::"; user code has access to this interface functions param type through config

protected:
	/**
		Map storing module-{moduleHandle:module_refs_amount} association
	*/
	std::map<TYPE_JOBMODID, std::pair<std::shared_ptr<IModuleContainer>, TYPE_SIZE>> modules;

public:
	virtual ~CModulesManager();
	/**
		Loads module if its not done yet; if module already loaded, increments its refs amount
	*/
	void loadModule(const TYPE_JOBMODID &);
	/**
		Decrements module refs amount and unloads it if amount becomes 0
	*/
	void unloadModule(const TYPE_JOBMODID &);

	std::shared_ptr<IModuleContainer> operator[](const TYPE_JOBMODID &) const;

//Exceptions
	class ExModulesManager : public ExEx { //TODO Rename of ExModulesContainer
		std::string exInfo;
	public:
		ExModulesManager(const std::string &par_exInfo);

		std::string getInfo() const override;
	};
};

#endif
