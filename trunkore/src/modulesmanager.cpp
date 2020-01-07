#include "modulesmanager.hpp"

//====================================================================================================
CModulesManager::~CModulesManager() {}

//====================================================================================================
std::shared_ptr<IModuleContainer> CModulesManager::operator[](const TYPE_JOBMODID& modID) const {
	decltype(this->modules)::const_iterator libIter = this->modules.find(modID);
	if (libIter == this->modules.cend()) throw ExModulesManager{ "No modules found" };
	return libIter->second.first;
}

//====================================================================================================
void CModulesManager::loadModule(const TYPE_JOBMODID & modID) {
	auto libIter = this->modules.find(modID);
	if (libIter == this->modules.end()) {
		auto insertIter = this->modules.insert({ modID,{ std::make_shared<config::TYPE_MODULECONTAINER>(),1 } }); //insert can get rvalue refs: created pair moved //TIP Specific module container object can be created through factory mechanism and then passed to Modules Manager
		this->modules[modID].first->loadModule(modID);
	}
	else ++(libIter->second.second);
}

//====================================================================================================
void CModulesManager::unloadModule(const TYPE_JOBMODID & modID) {
	auto libIter = this->modules.find(modID);
	if (libIter == this->modules.end()) throw ExModulesManager{ "No modules found" };
	if ((--(libIter->second.second)) == 0) {
		libIter->second.first->unloadModule();
		this->modules.erase(libIter);
	}
}

//CModulesManager::ExModulesContainer
//====================================================================================================
CModulesManager::ExModulesManager::ExModulesManager(const std::string & par_exInfo) : exInfo(par_exInfo) {}

//====================================================================================================
std::string CModulesManager::ExModulesManager::getInfo() const {
	return std::string("[CModulesManager EXCEPTION] ") + exInfo;
}
