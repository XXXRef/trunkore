#ifndef HEADER_JOBOWNER
#define HEADER_JOBOWNER

#include "i_jobowner.hpp"

#include "ext\blog\log.hpp"
#include "ext\blog\logger.hpp"

class CJobOwner : public IJobOwner{
public:
	enum EJobState { VANILLA, STEP1, STEP2, STEP3, STEP4, STEP5 }; //type declaration - static meaningless

protected:
	blog::CLogger logger;
	EJobState jobState = EJobState::VANILLA;

	std::string id;

public:
	CJobOwner(const std::string& par_id);
	virtual void init() override;
	virtual void deinit() override;
	virtual EPlayResult play() override;
};

#endif
