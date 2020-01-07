#ifndef HEADER_I_JOBOWNER
#define HEADER_I_JOBOWNER

#include <thread>
#include <mutex>
#include <chrono>
#include <exception>

#include "types.hpp"
#include "utils.hpp"

//=====================================================================================================================
template<class T> class CSharedVar {
protected:
	T var;
	std::mutex m;

public:
	CSharedVar(const T& obj = T{}) : var(obj) {}//TODO noexcept?
	//TODO take care bout copy-control members
	virtual ~CSharedVar() {}

	void set(const T& obj = T{}) {
		std::lock_guard<std::mutex>(this->m);
		this->var = obj;
	}

	T get() {
		std::lock_guard<std::mutex>(this->m);
		T tmpObj = this->var;
		return tmpObj;
	}
};

//=====================================================================================================================
/**
	\brief Class interface and partial implementation to handle job
	Job modules must inherit this class and implement init,deinit,play methods
*/
class IJobOwner {
public:
	/**
		\brief Set of all possible job states
	*/
	enum EExecState { VANILLA, INITIALIZED, PLAYING, STOPPED, ENDED, DEINITIALIZED, INTERRUPTED };

protected:
	enum EPlayResult { RESULT_STOPPED, RESULT_ENDED, RESULT_INTERRUPTED };
	CSharedVar<EExecState> executionState = EExecState::VANILLA;
	CSharedVar<bool> flagIfStopRequested = false; //TODO almost surely standard library implementations of this functionality exists //TODO in-body initializations ???
	std::mutex stateMutex;

	static constexpr TYPE_SIZE WAIT_TIME_MS = 100;//TODO use chrono for it

public:
	virtual ~IJobOwner() {
		if (EExecState::PLAYING == this->executionState.get()) this->onStop();
	}
	bool checkIfStopRequested() {//For users
		return this->flagIfStopRequested.get();
	}
	void playWrapper() {
		auto playResult = this->play();
		if (EPlayResult::RESULT_ENDED == playResult) this->executionState.set(EExecState::ENDED);
		else {//EPlayResult::RESULT_STOPPED
			this->flagIfStopRequested.set(false);
			this->executionState.set(EExecState::STOPPED);
		}
	}
	void onInit() {
		this->init();
		this->executionState.set(EExecState::INITIALIZED);
	}
	void onDeinit() {
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
	void onPlay() {
		auto curExecutionState = this->executionState.get();
		if ((curExecutionState == EExecState::ENDED) || (curExecutionState == EExecState::PLAYING) || (curExecutionState == EExecState::INTERRUPTED) || (curExecutionState == EExecState::DEINITIALIZED)) throw ExJobOwner("Invalid command sequence");
		this->executionState.set(EExecState::PLAYING);
		// Starting new thread for job
		auto t = std::thread(&IJobOwner::playWrapper, this);
		t.detach();
	}
	void onStop() {
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
	EExecState getExecutionState() {
		return this->executionState.get();
	}


	// User-provided methods which user should implement in separate source file (preferably not in i_jobowner.cpp )
	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual EPlayResult play() = 0;


	//Exceptions
	class ExJobOwner : public ExEx {
		std::string exInfo;
	public:
		ExJobOwner(const std::string& par_exInfo): exInfo(par_exInfo) {}

		std::string getInfo() const override {
			return std::string("[CJobsManager EXCEPTION] ") + this->exInfo;
		}
	};
};

#endif 
