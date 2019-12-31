#include "winmodulemanager.hpp"

//WinModuleManager
//====================================================================================================
CWinModuleManager::CWinModuleManager(): hModule(NULL) {}

//====================================================================================================
CWinModuleManager::CWinModuleManager(const TYPE_JOBMODID &modulePath): hModule(NULL) {
	this->loadModule(modulePath);
}

//====================================================================================================
void CWinModuleManager::loadModule(const TYPE_JOBMODID &modulePath) {
	this->unloadModule();
	auto hModule = LoadLibrary(static_cast<LPCTSTR>(modulePath.c_str()));
	if (NULL == hModule) throw ExWinModuleManager("Failed to load library");
	this->hModule = hModule;
}

//====================================================================================================
void CWinModuleManager::unloadModule() {
	if (NULL == this->hModule ) return;
	FreeLibrary(this->hModule);
	this->hModule = NULL;
}

//====================================================================================================
TYPE_SIZE CWinModuleManager::getFunctionAddress(const TYPE_JOBMODID &functionExportName) {
	auto functionAddr = reinterpret_cast<TYPE_SIZE>(GetProcAddress(this->hModule, functionExportName.c_str()));
	if (NULL == functionAddr) throw ExWinModuleManager("Failed to get module function address");
	return functionAddr;
}

//====================================================================================================
TYPE_SIZE CWinModuleManager::getFunctionAddress(WORD functionOrdinal) {
	auto functionAddr = reinterpret_cast<TYPE_SIZE>(GetProcAddress(this->hModule, reinterpret_cast<LPCSTR>(functionOrdinal)));
	if (NULL == functionAddr) throw ExWinModuleManager("Failed to get module function address (by ordinal)");
	return functionAddr;
}

//====================================================================================================
CWinModuleManager::~CWinModuleManager() {
	this->unloadModule();
}

//Exceptions
//====================================================================================================
CWinModuleManager::ExWinModuleManager::ExWinModuleManager(const std::string &par_msg) :_msg(par_msg), _errorCode(GetLastError()) {}

std::string CWinModuleManager::ExWinModuleManager::getInfo() const {
	return std::string("[CWinModuleManager EXCEPTION] ") + _msg + std::string(" err_code:") + std::to_string(_errorCode);
}


//CWinModuleManagerRAII
//====================================================================================================

CWinModuleManagerRAII::CWinModuleManagerRAII(const TYPE_JOBMODID &modulePath) {
	auto hModule = LoadLibrary(static_cast<LPCTSTR>(modulePath.c_str()));
	if (NULL == hModule) throw ExWinModuleManagerRAII("Failed to load library");
	this->hModule = hModule;
}

//====================================================================================================
TYPE_SIZE CWinModuleManagerRAII::getFunctionAddress(const TYPE_JOBMODID &functionExportName) {
	auto functionAddr = reinterpret_cast<TYPE_SIZE>(GetProcAddress(this->hModule, functionExportName.c_str()));
	if (NULL == functionAddr) throw ExWinModuleManagerRAII("Failed to get module function address");
	return functionAddr;
}

//====================================================================================================
TYPE_SIZE CWinModuleManagerRAII::getFunctionAddress(WORD functionOrdinal) {
	auto functionAddr = reinterpret_cast<TYPE_SIZE>(GetProcAddress(this->hModule, reinterpret_cast<LPCSTR>(functionOrdinal)));
	if (NULL == functionAddr) throw ExWinModuleManagerRAII("Failed to get module function address (by ordinal)");
	return functionAddr;
}

CWinModuleManagerRAII::~CWinModuleManagerRAII() {
	FreeLibrary(this->hModule);
}

//Exceptions
//====================================================================================================
CWinModuleManagerRAII::ExWinModuleManagerRAII::ExWinModuleManagerRAII(const std::string &par_msg) :_msg(par_msg), _errorCode(GetLastError()) {}

std::string CWinModuleManagerRAII::ExWinModuleManagerRAII::getInfo() const {
	return std::string("[CWinModuleManagerRAII EXCEPTION] ") + _msg + std::string(" err_code:") + std::to_string(_errorCode);
}
