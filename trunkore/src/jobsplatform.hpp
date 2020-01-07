#ifndef HEADER_JOBSPLATFORM
#define HEADER_JOBSPLATFORM

#include <string>
#include <map>
#include <utility>
#include <exception>
#include <vector>

#include "i_jobowner.hpp"
#include "jobsmanager.hpp"
#include "modulesmanager.hpp"
#include "utils.hpp"

#include "config.hpp"

//====================================================================================================
/**
	\brief Main JobsPlatform class which performs most of job handling business
*/
class CJobsPlatform {
public:
	using TYPE_JOBID=config::TYPE_JOBID;
	using TYPE_JOBMODID=config::TYPE_JOBMODID;
	using TYPE_JOBCONFIGID=config::TYPE_JOBCONFIGID;

private:
	/**
		Map holding jobID-jobLibID association
	*/
	std::map<TYPE_JOBID, TYPE_JOBMODID> jobsNlibs;

	CModulesManager modulesManager;

	CJobsManager jobsMgr;

public:
	//Interface
	virtual ~CJobsPlatform();
	void addJob(const TYPE_JOBID &, const TYPE_JOBMODID&, const TYPE_JOBCONFIGID &);
	void removeJob(const TYPE_JOBID &);
	void initJob(const TYPE_JOBID &);
	void deinitJob(const TYPE_JOBID &);
	void playJob(const TYPE_JOBID &);
	void stopJob(const TYPE_JOBID &);
	IJobOwner::EExecState getJobState(const TYPE_JOBID &); //TODO const?
	std::vector<TYPE_JOBID> getJobsIDs(); //TODO const?

//Types
	class ExJobsPlatform : public ExEx {
		std::string exInfo;
	public:
		ExJobsPlatform(const std::string &par_exInfo);

		std::string getInfo() const override;
	};
};

#endif
