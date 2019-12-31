#ifndef HEADER_JOBSMANAGER
#define HEADER_JOBSMANAGER

#include <map>
#include <exception>
#include <string>
#include <memory>

#include "i_jobowner.hpp"

using TYPE_JOB_ID=std::string;

//====================================================================================================
/**
	Class implementing core jobs processing
*/
class CJobsManager {
protected:
	/**
		Map holding {JOB_ID:JobOwner_obj} association
	*/
	std::map<TYPE_JOB_ID, IJobOwner*> jobs; //TODO Map contains raw ptr. Why no smart pointer

public:
	//TODO Constructor?
	virtual ~CJobsManager();
	void addJob(const TYPE_JOB_ID& jobID, IJobOwner*);
	void removeJob(const TYPE_JOB_ID& jobID);
	void initJob(const TYPE_JOB_ID& jobID);
	void deinitJob(const TYPE_JOB_ID& jobID);
	void playJob(const TYPE_JOB_ID& jobID);
	void stopJob(const TYPE_JOB_ID& jobID);
	IJobOwner::EExecState getJobState(const TYPE_JOB_ID& jobID); //TODO const?

//Exceptions
	/**
		Occurs when there is attempt to add job with JOB_ID which already exists
	*/
	class ExDuplicateJobsNames : public std::exception {};
	/**
		Occurs when there is attempt to select job with JOB_ID which doesnt exist
	*/
	class ExNoSuchJob : public std::exception {};
};

#endif