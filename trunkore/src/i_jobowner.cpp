#include "i_jobowner.hpp"

//=====================================================================================================================
IJobOwner::~IJobOwner() {
	if (EExecState::PLAYING == this->executionState.get()) {
		this->onStop();
	}
}

//=====================================================================================================================
bool IJobOwner::checkIfStopRequested() { //For users
	return this->flagIfStopRequested.get();
}

//=====================================================================================================================
void IJobOwner::playWrapper() {
	auto playResult = this->play();
	if (EPlayResult::RESULT_ENDED==playResult) this->executionState.set(EExecState::ENDED);
	else {//EPlayResult::RESULT_STOPPED
		this->flagIfStopRequested.set(false);
		this->executionState.set(EExecState::STOPPED);
	}
}

//=====================================================================================================================
void IJobOwner::onInit() {
	this->init();
	this->executionState.set(EExecState::INITIALIZED);
}

//=====================================================================================================================
void IJobOwner::onDeinit() {
	auto curExecutionState = this->executionState.get();
	switch (curExecutionState) {
	case EExecState::VANILLA:
	case EExecState::DEINITIALIZED:
		throw ExJobOwner("Invalid command sequence");
	case EExecState::PLAYING:
		this->onStop();
		break;
	case EExecState::ENDED:
		break;
	}
	// Calling user-provided function
	this->deinit();
	this->executionState.set(EExecState::DEINITIALIZED);
}

//=====================================================================================================================
void IJobOwner::onPlay() {
	auto curExecutionState = this->executionState.get();
	if ((curExecutionState == EExecState::ENDED) || (curExecutionState == EExecState::PLAYING) || (curExecutionState == EExecState::INTERRUPTED) || (curExecutionState == EExecState::DEINITIALIZED)) throw ExJobOwner("Invalid command sequence");
	this->executionState.set(EExecState::PLAYING);
	// Starting new thread for job
	auto t = std::thread(&IJobOwner::playWrapper, this);
	t.detach();
}

//=====================================================================================================================
void IJobOwner::onStop() {
	if (this->executionState.get() == EExecState::ENDED) throw ExJobOwner("Job already ended");
	if (this->executionState.get() != EExecState::PLAYING) throw ExJobOwner("Invalid command sequence");

	this->flagIfStopRequested.set(true);
	// Wait until state says job stopped/ended/interrupted (not playing)
	while (true) {
		if (this->executionState.get() != EExecState::PLAYING) break;
		std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME_MS));
	}
	//thread sets state with STOPPED by itself; we can just wait till thread end to make sure state is set to stopped
	//If thread was detached, then we needed checking job owner state in loop
}

//=====================================================================================================================
EExecState IJobOwner::getExecutionState() inline {
	return this->executionState.get();
}


//Exceptions
//====================================================================================================
IJobOwner::ExJobOwner::ExJobOwner(const std::string& par_exInfo) inline: exInfo(par_exInfo) {}

//====================================================================================================
std::string IJobOwner::ExJobOwner::getInfo() const inline {
	return std::string("[CJobsManager EXCEPTION] ") + this->exInfo;
}
