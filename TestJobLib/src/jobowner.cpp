#include "jobowner.hpp"

#include <windows.h>

#include <string>

CJobOwner::CJobOwner(const std::string& par_id) : id(par_id){
	blog::addEndpoint(std::string("TestJobLib_") + this->id + ".log", std::string("logfile_") + this->id);
	this->logger.addEndpoint(std::string("logfile_")+this->id);
}

void CJobOwner::init(){
	std::size_t secsToSleep = 3;
	this->logger.log("START CJobOwner::init");
	this->jobState = EJobState::VANILLA;
	//this->logger.log(std::string("\tSleeping for ")+std::to_string(secsToSleep)+" secs ...");
	//Sleep(secsToSleep*1000);
	this->logger.log("END CJobOwner::init");
}

void CJobOwner::deinit(){
	std::size_t secsToSleep = 3;
	this->logger.log("START CJobOwner::deinit");
	//this->logger.log(std::string("\tSleeping for ") + std::to_string(secsToSleep) + " secs ...");
	//Sleep(secsToSleep * 1000);
	this->logger.log("END CJobOwner::deinit");
}

CJobOwner::EPlayResult CJobOwner::play(){
	switch (this->jobState){
	case EJobState::VANILLA:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		this->logger.log("VANILLA done");
		this->jobState = EJobState::STEP1;

	case EJobState::STEP1:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		this->logger.log("STEP1 done");
		this->jobState = EJobState::STEP2;

	case EJobState::STEP2:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		this->logger.log("STEP2 done");
		this->jobState = EJobState::STEP3;

	case EJobState::STEP3:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		this->logger.log("STEP3 done");
		this->jobState = EJobState::STEP4;

	case EJobState::STEP4:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		this->logger.log("STEP4 done");
		this->jobState = EJobState::STEP5;
	}
	return EPlayResult::RESULT_ENDED;
}