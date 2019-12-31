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
	auto jobsNames = this->getJobsNames();
	for (const auto& e: jobsNames){
		this->removeJob(e);
	}
}

//====================================================================================================
void CJobsPlatform::addJob(const std::string& jobName, const std::string& jobLibPath, const std::string& jobConfigPath) {
// Check for duplicate
	if (this->jobsNlibs.find(jobName) != this->jobsNlibs.cend()) {
		throw ExDuplicateJobs();
	}
// Load job library
	try {
		this->modulesContainer.loadModule(jobLibPath);
	}
	catch (const ExEx& e) {
		std::cout << "[ERROR]: " << "Exception while loading module occurred: " << e.getInfo() << std::endl;
		//TODO thow CJobPlatform exception
		return;
	}

// Get job factory function
	IJobOwner*(*pfnGetJob)(const std::string&);
	try {
		pfnGetJob = (IJobOwner*(*)(const std::string&))this->modulesContainer[jobLibPath]->getFunctionAddress(config::getJobFunctionName);
	}
	catch (const ExEx& e) {
		this->modulesContainer.unloadModule(jobLibPath);
		std::cout << "[ERROR]: " << "Exception while loading module occurred: " << e.getInfo() << std::endl;
		//TODO thow CJobPlatform exception
		return;
	}

// Get jobowner
	auto jobManager = pfnGetJob(jobConfigPath);
	if (jobManager == nullptr) {
		this->modulesContainer.unloadModule(jobLibPath);
		std::cout << "[ERROR]: " << "Cant get job manager object" << std::endl;
		//TODO thow CJobPlatform exception
		return;
	}

// Insert new job element after job manager object successfully acquired
	this->jobsNlibs.insert({ jobName,jobLibPath });

// Create job
	this->jobsMgr.addJob(jobName, jobManager); // No need for handling exceptions cos checking for name duplication is already done in the beginning
}

//====================================================================================================
void CJobsPlatform::removeJob(const std::string& jobName){
// Delete job
	try{
		this->jobsMgr.removeJob(jobName);
	}
	catch (CJobsManager::ExNoSuchJob){
		throw CJobsPlatform::ExNoJobFound();
	}
// Unload / decrement use_amount of job module
	auto iter = this->jobsNlibs.find(jobName);
	this->modulesContainer.unloadModule(iter->second);
// Remove appropriate element from this->jobsNlibs
	this->jobsNlibs.erase(iter);
}

//====================================================================================================
void CJobsPlatform::initJob(const std::string& jobName){
	try{
		this->jobsMgr.initJob(jobName);
	}
	catch (CJobsManager::ExNoSuchJob){
		throw CJobsPlatform::ExNoJobFound();
	}
}

//====================================================================================================
void CJobsPlatform::deinitJob(const std::string& jobName){
	try{
		this->jobsMgr.deinitJob(jobName);
	}
	catch (CJobsManager::ExNoSuchJob){
		throw CJobsPlatform::ExNoJobFound();
	}
}

//====================================================================================================
void CJobsPlatform::playJob(const std::string& jobName){
	try{
		this->jobsMgr.playJob(jobName);
	}
	catch (const CJobsManager::ExNoSuchJob& e){
		throw CJobsPlatform::ExNoJobFound();
	}
	catch (const IJobOwner::ExInvalidCommandSequence& e){
		std::cout << "CJobsPlatform::playJob ERROR: job state prevents playing it" << std::endl;
		//TODO thow CJobPlatform exception
		return;
	}
}

//====================================================================================================
void CJobsPlatform::stopJob(const std::string& jobName){
	try{
		this->jobsMgr.stopJob(jobName);
	}
	catch (CJobsManager::ExNoSuchJob){
		throw CJobsPlatform::ExNoJobFound();
	}
	catch (const IJobOwner::ExJobAlreadyEnded& e){
		std::cout << "CJobsPlatform::stopJob ERROR: job already ended" << std::endl;
		//TODO thow CJobPlatform exception
		return;
	}
}

//====================================================================================================
std::vector<std::string> CJobsPlatform::getJobsNames() {
	std::vector<std::string> jobsNames;
	for (const auto& e: this->jobsNlibs) {
		jobsNames.push_back(e.first);
	}
	return jobsNames;
}

//====================================================================================================
IJobOwner::EExecState CJobsPlatform::getJobState(const std::string& jobName){
	try{
		this->jobsMgr.getJobState(jobName);
	}
	catch (CJobsManager::ExNoSuchJob){
		throw CJobsPlatform::ExNoJobFound();
	}
}