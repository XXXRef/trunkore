#include "jobsmanager.hpp"

//=====================================================================================================================
CJobsManager::~CJobsManager() {
	// Deinit, remove all remaining jobs
	for (auto iter = this->jobs.begin(); iter != this->jobs.end(); ++iter) {
		try {
			this->removeJob(iter->first);
		}
		catch (...) {
			this->jobs.erase(iter);
		}
	}
}

//=====================================================================================================================
void CJobsManager::addJob(const TYPE_JOB_ID& jobID, IJobOwner* pJobOwner) {
	if (this->jobs.find(jobID) != this->jobs.cend()) {
		throw ExDuplicateJobsNames();
	}
	this->jobs.insert({jobID, pJobOwner});
}

//=====================================================================================================================
void CJobsManager::removeJob(const TYPE_JOB_ID& jobID) {
	auto iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) {
		throw ExNoSuchJob();
	}
	// Try to deinitialize job being removed
	try {
		iter->second->onDeinit();
	}
	catch (IJobOwner::ExInvalidCommandSequence& e) {
		// Now its safe to remove job. Nothing to do.
	}
	// Remove job element
	delete iter->second;
	this->jobs.erase(iter);
}

//=====================================================================================================================
void CJobsManager::initJob(const TYPE_JOB_ID& jobID) {
	auto iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) {
		throw ExNoSuchJob();
	}
	iter->second->onInit(); // What if exception?
}

//=====================================================================================================================
void CJobsManager::deinitJob(const TYPE_JOB_ID& jobID) {
	auto iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) {
		throw ExNoSuchJob();
	}
	iter->second->onDeinit(); // What if exception?
}

//=====================================================================================================================
void CJobsManager::playJob(const TYPE_JOB_ID& jobID) {
	auto iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) {
		throw ExNoSuchJob();
	}
	iter->second->onPlay(); // What if exception?
}

//=====================================================================================================================
void CJobsManager::stopJob(const TYPE_JOB_ID& jobID) {
	auto iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) {
		throw ExNoSuchJob();
	}
	iter->second->onStop(); // What if exception?
}

//=====================================================================================================================
IJobOwner::EExecState CJobsManager::getJobState(const TYPE_JOB_ID& jobID) {
	auto iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) {
		throw ExNoSuchJob();
	}
	return iter->second->getExecutionState();
}