#ifndef HEADER_MODULESCONTAINER
#define HEADER_MODULESCONTAINER

#include <map>
#include <utility>
#include <string>
#include <memory>

#include <exception>

#include "config.hpp"

#include "types.hpp"
#include "utils.hpp"
#include "ModuleManager/i_modulemanager.hpp"

//====================================================================================================
/**
	\brief Handles job modules
*/
template<class ClassModuleManager> class CModulesContainer {
public: // public types

protected:
	/**
		Map storing moduleID-{moduleHandle:module_refs_amount} association
	*/
	std::map<TYPE_JOBMODID, std::pair<std::shared_ptr<ClassModuleManager>, TYPE_SIZE>> modules; //module ID must handle widestrings //TODO ClassModuleManager ptr lol

public:
	//TODO copy control members
	virtual ~CModulesContainer();
	/**
		Loads module if its not done yet; if module already loaded, increments its refs amount
	*/
	void loadModule(const TYPE_JOBMODID &);
	/**
		Decrements module refs amount and unloads it if amount becomes 0
	*/
	void unloadModule(const TYPE_JOBMODID &);

	std::shared_ptr<ClassModuleManager> operator[](const TYPE_JOBMODID &) const;

//Exceptions
	class ExModulesContainer : public ExEx { //TODO Rename of ExModulesContainer
		std::string exInfo;
	public:
		ExModulesContainer(const std::string &par_exInfo):exInfo(par_exInfo) {}

		std::string getInfo() const override {
			return std::string("[CModulesContainer EXCEPTION] ")+exInfo;
		}
	};
};

//Template class methods implementation must be available for compiler => its in header file
//====================================================================================================
template<class ClassModuleManager> CModulesContainer<ClassModuleManager>::~CModulesContainer() = default;

//====================================================================================================
template<class ClassModuleManager> std::shared_ptr<ClassModuleManager> CModulesContainer<ClassModuleManager>::operator[](const TYPE_JOBMODID &libID) const {
	decltype(this->modules)::const_iterator libIter = this->modules.find(libID);
	if (libIter == this->modules.cend()) throw ExModulesContainer{"No modules found"};
	return libIter->second.first;
}

//====================================================================================================
template<class ClassModuleManager> void CModulesContainer<ClassModuleManager>::loadModule(const TYPE_JOBMODID &libID) {
	auto libIter = this->modules.find(libID);
	if (libIter == this->modules.end()) this->modules.insert({ libID,{ std::make_shared<ClassModuleManager>(libID),1 } });
	else ++(libIter->second.second);
}

//====================================================================================================
template<class ClassModuleManager> void CModulesContainer<ClassModuleManager>::unloadModule(const TYPE_JOBMODID &libID) {
	auto libIter = this->modules.find(libID);
	if (libIter == this->modules.end()) throw ExModulesContainer{ "No modules found" };
	if ((--(libIter->second.second))==0) this->modules.erase(libIter);
}

#endif