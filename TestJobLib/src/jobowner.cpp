#include "jobowner.hpp"

#include <windows.h>

#include <string>

CJobOwner::CJobOwner(const LPCWSTR &par_id){
	this->hLogFile=CreateFile(reinterpret_cast<LPCSTR>(par_id), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

CJobOwner::~CJobOwner() {
	CloseHandle(this->hLogFile);
}

void CJobOwner::init(){
	std::size_t secsToSleep = 3;
	DWORD numBytesWritten=0;
	WriteFile(this->hLogFile, "START CJobOwner::init", std::strlen("START CJobOwner::init"), &numBytesWritten, nullptr);
	this->jobState = EJobState::VANILLA;
	WriteFile(this->hLogFile, "END CJobOwner::init", std::strlen("END CJobOwner::init"), &numBytesWritten, nullptr);
}

void CJobOwner::deinit(){
	std::size_t secsToSleep = 3;
	DWORD numBytesWritten = 0;
	WriteFile(this->hLogFile, "START CJobOwner::deinit", std::strlen("START CJobOwner::deinit"), &numBytesWritten, nullptr);
	//this->logger.log(std::string("\tSleeping for ") + std::to_string(secsToSleep) + " secs ...");
	//Sleep(secsToSleep * 1000);
	WriteFile(this->hLogFile, "END CJobOwner::deinit", std::strlen("END CJobOwner::deinit"), &numBytesWritten, nullptr);
}

CJobOwner::EPlayResult CJobOwner::play(){
	DWORD numBytesWritten = 0;
	switch (this->jobState){
	case EJobState::VANILLA:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		WriteFile(this->hLogFile, "VANILLA done", std::strlen("VANILLA done"), &numBytesWritten, nullptr);
		this->jobState = EJobState::STEP1;

	case EJobState::STEP1:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		WriteFile(this->hLogFile, "STEP1 done", std::strlen("STEP1 done"), &numBytesWritten, nullptr);
		this->jobState = EJobState::STEP2;

	case EJobState::STEP2:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		WriteFile(this->hLogFile, "STEP2 done", std::strlen("STEP2 done"), &numBytesWritten, nullptr);
		this->jobState = EJobState::STEP3;

	case EJobState::STEP3:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		WriteFile(this->hLogFile, "STEP3 done", std::strlen("STEP3 done"), &numBytesWritten, nullptr);
		this->jobState = EJobState::STEP4;

	case EJobState::STEP4:
		if (this->flagIfStopRequested.get()){
			return EPlayResult::RESULT_STOPPED;
		}
		Sleep(2000);
		WriteFile(this->hLogFile, "STEP4 done", std::strlen("STEP4 done"), &numBytesWritten, nullptr);
		this->jobState = EJobState::STEP5;
	}
	return EPlayResult::RESULT_ENDED;
}