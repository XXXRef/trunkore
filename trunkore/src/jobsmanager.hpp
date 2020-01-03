#ifndef HEADER_JOBSMANAGER
#define HEADER_JOBSMANAGER

#include <map>
#include <exception>
#include <string>
#include <memory>

#include "i_jobowner.hpp"
#include "utils.hpp"

#include "config.hpp"

//====================================================================================================
/**
	Class implementing core jobs processing
*/
class CJobsManager {
public:
	using TYPE_JOBID=config::TYPE_JOBID;

protected:
	/**
		Map holding {JOB_ID:JobOwner_obj} association
	*/
	std::map<TYPE_JOBID, std::shared_ptr<IJobOwner>> jobs;

public:
	//TODO Constructor?
	virtual ~CJobsManager();
	void addJob(const TYPE_JOBID& jobID, const std::shared_ptr<IJobOwner> &);
	void removeJob(const TYPE_JOBID& jobID);
	void initJob(const TYPE_JOBID& jobID);
	void deinitJob(const TYPE_JOBID& jobID);
	void playJob(const TYPE_JOBID& jobID);
	void stopJob(const TYPE_JOBID& jobID);
	IJobOwner::EExecState getJobState(const TYPE_JOBID &jobID); //TODO const?

//Exceptions
	class ExJobsManager : public ExEx {
		std::string exInfo;

	public:
		ExJobsManager(const std::string& par_exInfo);

		std::string getInfo() const override;
	};
};

#endif
