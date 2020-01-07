#ifndef HEADER_JOBOWNER
#define HEADER_JOBOWNER

#include "EXT/trunkore/inc/i_jobowner.hpp"

#include <windows.h>

class CJobOwner : public IJobOwner{
public:
	enum EJobState { VANILLA, STEP1, STEP2, STEP3, STEP4, STEP5 }; //type declaration - static meaningless

protected:
	EJobState jobState = EJobState::VANILLA;

	HANDLE hLogFile;

public:
	CJobOwner(const LPCWSTR &par_id);
	virtual void init() override;
	virtual void deinit() override;
	virtual EPlayResult play() override;
	~CJobOwner() override;
};

#endif
