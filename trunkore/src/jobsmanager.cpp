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
void CJobsManager::addJob(const TYPE_JOBID& jobID, const std::shared_ptr<IJobOwner> &pJobOwner) {
	auto insertResult=this->jobs.insert({jobID, pJobOwner});
	if (false==insertResult.second) throw ExJobsManager("Failed to add job: job with this jobID already exists");//TIP need to output jobID in exception message
}

//=====================================================================================================================
void CJobsManager::removeJob(const TYPE_JOBID &jobID) {
	decltype(this->jobs)::const_iterator iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) throw ExJobsManager("Job removing failed: there is no job with this jobID");//TIP need to output jobID in exception message
	// Try to deinitialize job being removed
	try {
		iter->second->onDeinit();//TODO why deinit while removing called?
	}
	catch (IJobOwner::ExJobOwner &e) {
		// Now its safe to remove job. Nothing to do.
	}
	// Remove job element
	this->jobs.erase(iter);
}

//=====================================================================================================================
void CJobsManager::initJob(const TYPE_JOBID& jobID) {
	decltype(this->jobs)::const_iterator iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) throw ExJobsManager("Job initializing failed: there is no job with this jobID");//TODO change IJobOwner exception type when common IJobOwner exception type will be implemented
	iter->second->onInit(); //TODO What if exception?
}

//=====================================================================================================================
void CJobsManager::deinitJob(const TYPE_JOBID& jobID) {
	decltype(this->jobs)::const_iterator iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) throw ExJobsManager("Job deinitializing failed: there is no job with this jobID");//TODO change IJobOwner exception type when common IJobOwner exception type will be implemented
	iter->second->onDeinit(); //TODO What if exception?
}

//=====================================================================================================================
void CJobsManager::playJob(const TYPE_JOBID& jobID) {
	decltype(this->jobs)::const_iterator iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) throw ExJobsManager("Job playing failed: there is no job with this jobID");//TODO change IJobOwner exception type when common IJobOwner exception type will be implemented
	iter->second->onPlay(); //TODO What if exception?
}

//=====================================================================================================================
void CJobsManager::stopJob(const TYPE_JOBID& jobID) {
	decltype(this->jobs)::const_iterator iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) throw ExJobsManager("Job stopping failed: there is no job with this jobID");//TODO change IJobOwner exception type when common IJobOwner exception type will be implemented
	iter->second->onStop(); //TODO What if exception?
}

//=====================================================================================================================
IJobOwner::EExecState CJobsManager::getJobState(const TYPE_JOBID& jobID) {
	decltype(this->jobs)::const_iterator iter = this->jobs.find(jobID);
	if (iter == this->jobs.cend()) throw ExJobsManager("Job state acquiring failed: there is no job with this jobID");//TODO change IJobOwner exception type when common IJobOwner exception type will be implemented
	return iter->second->getExecutionState();
}

//Exceptions
//====================================================================================================
CJobsManager::ExJobsManager::ExJobsManager(const std::string& par_exInfo) inline: exInfo(par_exInfo) {}

//====================================================================================================
std::string CJobsManager::ExJobsManager::getInfo() const inline {
	return std::string("[CJobsManager EXCEPTION] ") + this->exInfo;
}
