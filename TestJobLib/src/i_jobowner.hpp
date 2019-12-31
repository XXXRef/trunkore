#ifndef HEADER_I_JOBOWNER
#define HEADER_I_JOBOWNER

#include <thread>
#include <mutex>
#include <chrono>
#include <exception>

#include "types.hpp"
//===================================================================================================================== CSharedVar
template<class T> class CSharedVar {
protected:
	T var;
	std::mutex m;

public:
	CSharedVar(const T& obj = T{}) : var(obj) {}

	void set(const T& obj = T{}) {
		this->m.lock();
		this->var = obj;
		this->m.unlock();
	}

	T get() {
		this->m.lock();
		T tmpObj = this->var;
		this->m.unlock();
		return tmpObj;
	}
};

//===================================================================================================================== IJobOwner
class IJobOwner {
public:
	enum EExecState { VANILLA, INITIALIZED, PLAYING, STOPPED, ENDED, DEINITIALIZED, INTERRUPTED };
	class ExInvalidCommandSequence : public std::exception {};
	class ExJobAlreadyEnded : public std::exception {};

protected:
	enum EPlayResult { RESULT_STOPPED, RESULT_ENDED, RESULT_INTERRUPTED };
	CSharedVar<EExecState> executionState = EExecState::VANILLA;
	CSharedVar<bool> flagIfStopRequested = false;
	std::mutex stateMutex;

	static const TYPE_SIZE WAIT_TIME_MS = 100;

public:
	virtual ~IJobOwner() {
		if (this->executionState.get() == EExecState::PLAYING) {
			this->onStop();
		}
	}

	bool checkIfStopRequested() { //For users
		return this->flagIfStopRequested.get();
	}

	/* playWrapper works directly with flag so this method is unneccessary
	bool requestStop() {
	this->flagIfStopRequested.set(true);
	}
	*/
	// User-provided methods
	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual EPlayResult play() = 0;

	void playWrapper() {
		auto playResult = this->play();
		this->stateMutex.lock();
		if (playResult == EPlayResult::RESULT_ENDED) {
			this->executionState.set(EExecState::ENDED);
		}
		else {// EPlayResult::RESULT_STOPPED
			this->flagIfStopRequested.set(false);
			this->executionState.set(EExecState::STOPPED);
		}
		//EPlayResult::RESULT_INTERRUPTED
		this->stateMutex.unlock();
	}

	void onInit() {
		this->init();
		this->stateMutex.lock();
		this->executionState.set(EExecState::INITIALIZED);
		this->stateMutex.unlock();
	}

	void onDeinit() {
		this->stateMutex.lock();
		auto curExecutionState = this->executionState.get();
		switch (curExecutionState) {
		case EExecState::VANILLA:
		case EExecState::DEINITIALIZED:
			throw ExInvalidCommandSequence();
		case EExecState::PLAYING:
			this->onStop();
			break;
		case EExecState::ENDED:
			break;
		}
		/*
		if ((curExecutionState == EExecState::DEINITIALIZED) || (curExecutionState == EExecState::VANILLA)) {
		throw ExInvalidCommandSequence();
		}
		// Stop job thread if its running
		if (curExecutionState == EExecState::PLAYING) {
		this->executionState.set(EExecState::STOPPED);
		this->t.join();
		}
		*/
		// Calling user-provided function
		this->deinit();
		this->executionState.set(EExecState::DEINITIALIZED);
	}

	void onPlay() {
		auto curExecutionState = this->executionState.get();
		if ((curExecutionState == EExecState::ENDED) || (curExecutionState == EExecState::PLAYING) || (curExecutionState == EExecState::INTERRUPTED) || (curExecutionState == EExecState::DEINITIALIZED)) {
			throw ExInvalidCommandSequence();
		}
		this->executionState.set(EExecState::PLAYING);
		// Starting new thread for job
		auto t = std::thread(&IJobOwner::playWrapper, this);
		t.detach();
	}

	void onStop() {
		auto curExecutionState = this->executionState.get();
		if (curExecutionState == EExecState::ENDED) {
			throw ExJobAlreadyEnded();
		}
		if (this->executionState.get() != EExecState::PLAYING) {
			throw ExInvalidCommandSequence();
		}

		this->flagIfStopRequested.set(true);
		// Wait until state says job stopped/ended/interrupted (not playing)
		while (true) {
			if (this->executionState.get() != EExecState::PLAYING) {
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME_MS));
		}
		//thread sets state with STOPPED by itself; we can just wait till thread end to make sure state is set to stopped
		//If thread was detached, then we needed checking job owner state in loop
	}

	EExecState getExecutionState() {
		return this->executionState.get();
	}
};

#endif 
