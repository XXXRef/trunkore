#include "jobsplatform.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include "utils.hpp"
#include "types.hpp"
#include "config.hpp"
#include "i_jobowner.hpp"

//====================================================================================================
CJobsPlatform::~CJobsPlatform() {
	// Stop all remaining jobs
	auto jobsIDs = this->getJobsIDs();
	for (const auto &e : jobsIDs) {
		this->removeJob(e);
	}
}

//====================================================================================================
void CJobsPlatform::addJob(const TYPE_JOBID &jobID, const TYPE_JOBMODID &jobModID, const TYPE_JOBCONFIGID &jobConfigID) {
	// Check if duplicate
	if (this->jobsNlibs.find(jobID) != this->jobsNlibs.cend()) throw ExJobsPlatform("Adding job failed: job with this jobID already exists");

	// Load job library
	try {
		this->modulesManager.loadModule(jobModID);
	}
	catch (const CModulesManager::ExModulesManager & e) {
		throw ExJobsPlatform(std::string("ModulesManager module loading failed: ") + e.getInfo());
	}

	// Get job factory function
	IJobOwner* (*pfnGetJob)(const TYPE_JOBCONFIGID &);
	try {
		pfnGetJob = reinterpret_cast<decltype(pfnGetJob)>(this->modulesManager[jobModID]->getFunctionAddress(config::getJobFunctionName));
	}
	catch (const ExModuleContainer &e) {
		this->modulesManager.unloadModule(jobModID);
		throw ExJobsPlatform(std::string("Get func address failed: ") + e.getInfo());
	}

	// Get jobowner
	//auto pJobManagerObj = std::make_shared<std::remove_reference<decltype(*pfnGetJob(jobConfigID))>::type>(pfnGetJob(jobConfigID));//lol im loving it
	auto pJobManagerObj = std::shared_ptr<IJobOwner>(pfnGetJob(jobConfigID));//grap raw IJobOwner ptr, not construct it with make_shared
	if (pJobManagerObj == nullptr) {
		this->modulesManager.unloadModule(jobModID);
		throw ExJobsPlatform("Cant get job manager object");
	}

	// Insert new job element after job manager object successfully acquired
	this->jobsNlibs.insert({ jobID,jobModID });

	// Create job
	this->jobsMgr.addJob(jobID, pJobManagerObj);
}

//====================================================================================================
void CJobsPlatform::removeJob(const TYPE_JOBID &jobID) {
	// Delete job
	try {
		this->jobsMgr.removeJob(jobID);
	}
	catch (CJobsManager::ExJobsManager &e) {
		throw ExJobsPlatform(std::string("Job removing failed: ") + e.getInfo());
	}
	// Unload / decrement use_amount of job module
	auto iter = this->jobsNlibs.find(jobID); //if no exceptions jobsMgr business => jobID guaranteed to be in jobsNlibs => no checking for existence of job needed
	this->modulesManager.unloadModule(iter->second);
	// Remove appropriate element from this->jobsNlibs
	this->jobsNlibs.erase(iter);
}

//====================================================================================================
void CJobsPlatform::initJob(const TYPE_JOBID &jobID) {
	try {
		this->jobsMgr.initJob(jobID);
	}
	catch (CJobsManager::ExJobsManager &e) {
		throw ExJobsPlatform(std::string("Job initialization failed: ") + e.getInfo());
	}
}

//====================================================================================================
void CJobsPlatform::deinitJob(const TYPE_JOBID &jobID) {
	try {
		this->jobsMgr.deinitJob(jobID);
	}
	catch (CJobsManager::ExJobsManager &e) {
		throw ExJobsPlatform(std::string("Job deinitialization failed: ") + e.getInfo());
	}
}

//====================================================================================================
void CJobsPlatform::playJob(const TYPE_JOBID &jobID) {
	try {
		this->jobsMgr.playJob(jobID);
	}
	catch (const CJobsManager::ExJobsManager &e) {
		throw ExJobsPlatform(std::string("Job playing failed: ") + e.getInfo());
	}
	catch (const IJobOwner::ExJobOwner &e) {
		//std::cout << "CJobsPlatform::playJob ERROR: job state prevents playing it" << std::endl;
		throw ExJobsPlatform(std::string("Playing job failed: ") + e.what());//TODO remove what() when IJobOwner::ExInvalidCommandSequence will be reworked
	}
}

//====================================================================================================
void CJobsPlatform::stopJob(const TYPE_JOBID &jobID) {
	try {
		this->jobsMgr.stopJob(jobID);
	}
	catch (const CJobsManager::ExJobsManager &e) {
		throw ExJobsPlatform(std::string("Job stopping failed: ") + e.getInfo());
	}
	catch (const IJobOwner::ExJobOwner &e) {
		//std::cout << "CJobsPlatform::stopJob ERROR: job already ended" << std::endl;
		throw ExJobsPlatform(std::string("Stopping job failed: ") + e.what());//TODO remove what() when IJobOwner::ExInvalidCommandSequence will be reworked
	}
}

//====================================================================================================
auto CJobsPlatform::getJobsIDs() -> std::vector<TYPE_JOBID> {
	std::vector<decltype(this->jobsNlibs)::key_type> jobsIDs(this->jobsNlibs.size());
	for (const auto &e : this->jobsNlibs) jobsIDs.push_back(e.first);
	return jobsIDs;
}

//====================================================================================================
IJobOwner::EExecState CJobsPlatform::getJobState(const TYPE_JOBID &jobID) {
	IJobOwner::EExecState jobState;
	try {
		jobState = this->jobsMgr.getJobState(jobID);
	}
	catch (const CJobsManager::ExJobsManager &e) {
		throw ExJobsPlatform(std::string("Acquiring job state failed: ") + e.getInfo());
	}
	return jobState;
}


//CJobsPlatform::ExJobsPlatform
//====================================================================================================
CJobsPlatform::ExJobsPlatform::ExJobsPlatform(const std::string &par_exInfo): exInfo(par_exInfo) {}

//====================================================================================================
std::string CJobsPlatform::ExJobsPlatform::getInfo() const {
	return std::string("[CJobsPlatform EXCEPTION] ") + exInfo;
}
