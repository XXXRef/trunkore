#ifndef HEADER_WINMODULEMANAGER
#define HEADER_WINMODULEMANAGER

#include "i_modulemanager.hpp"

#include <exception>

#include <windows.h>

//=====================================================================================================================
class CWinModuleManager:public IModuleManager {
	HMODULE hModule;

public:
	CWinModuleManager();
	CWinModuleManager(const TYPE_JOBMODID &modulePath);
	void loadModule(const TYPE_JOBMODID &modulePath) override;
	void unloadModule() override;
	TYPE_SIZE getFunctionAddress(const TYPE_FUNCNAME &functionExportName) override;
	TYPE_SIZE getFunctionAddress(WORD functionOrdinal);// Get function address by ordinal
	~CWinModuleManager() override;

//Exceptions
	class ExWinModuleManager : public IModuleManager::ExModuleManager {
	public:
		DWORD _errorCode;
		std::string _msg;

		ExWinModuleManager(const std::string& par_msg = "");
		std::string getInfo() const override;
	};
};

//=====================================================================================================================
class CWinModuleManagerRAII : public IModuleManagerRAII {
	HMODULE hModule;
public:

	CWinModuleManagerRAII(const TYPE_JOBMODID &modulePath);
	TYPE_SIZE getFunctionAddress(const TYPE_FUNCNAME &) override;
	TYPE_SIZE getFunctionAddress(WORD functionOrdinal);// Get function address by ordinal
	~CWinModuleManagerRAII();

//Exceptions
	class ExWinModuleManagerRAII : public IModuleManagerRAII::ExModuleManagerRAII {
	public:
		DWORD _errorCode;
		std::string _msg;

		ExWinModuleManagerRAII(const std::string& par_msg = "");
		std::string getInfo() const override;
	};
};

#endif