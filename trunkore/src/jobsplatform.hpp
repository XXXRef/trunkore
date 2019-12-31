#ifndef HEADER_JOBSPLATFORM
#define HEADER_JOBSPLATFORM

#include <string>
#include <map>
#include <utility>
#include <exception>
#include <vector>

#include "i_jobowner.hpp"
#include "jobsmanager.hpp"
#if defined PLATFORM_WINDOWS
#include "ModuleManager/winmodulemanager.hpp"
using CLASS_MODULEMANAGER = CWinModuleManager;
#elif defined PLATFORM_NIX
#include "ModuleManager/nixmodulemanager.hpp"
using CLASS_MODULEMANAGER = CNixModuleManager;
#endif
#include "modulescontainer.hpp"

//====================================================================================================
/**
	\brief Main JobsPlatform class which performs most of job handling business
*/
class CJobsPlatform {
	/**
		Map holding jobID-jobLibID association
	*/
	std::map<std::string /*jobName*/, std::string /*jobLibPath*/> jobsNlibs;

	CModulesContainer<CLASS_MODULEMANAGER> modulesContainer;

	CJobsManager jobsMgr;

public:
//Interface
	virtual ~CJobsPlatform();
	void CJobsPlatform::addJob(const std::string& jobName, const std::string& jobLibPath, const std::string& jobConfigPath);
	void CJobsPlatform::removeJob(const std::string& jobName);
	void CJobsPlatform::initJob(const std::string& jobName);
	void CJobsPlatform::deinitJob(const std::string& jobName);
	void CJobsPlatform::playJob(const std::string& jobName);
	void CJobsPlatform::stopJob(const std::string& jobName);
	IJobOwner::EExecState getJobState(const std::string& jobName); //TODO const?
	std::vector<std::string> getJobsNames(); //TODO const?
//Types
	class ExDuplicateJobs : public std::exception {}; //TODO Replace on ExJobsPlatform
	class ExNoJobFound : public std::exception {};
};

#endif
