#ifndef HEADER_I_JOBOWNER
#define HEADER_I_JOBOWNER

#include <thread>
#include <mutex>
#include <chrono>
#include <exception>

#include "types.hpp"

//=====================================================================================================================
template<class T> class CSharedVar {
protected:
	T var;
	std::mutex m;

public:
	CSharedVar(const T &obj = T{}) : var(obj) {}//TODO noexcept?
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
	enum EPlayResult {RESULT_STOPPED, RESULT_ENDED, RESULT_INTERRUPTED};
	CSharedVar<EExecState> executionState = EExecState::VANILLA;
	CSharedVar<bool> flagIfStopRequested = false; //TODO almost surely standard library implementations of this functionality exists //TODO in-body initializations ???
	std::mutex stateMutex;

	static constexpr TYPE_SIZE WAIT_TIME_MS = 100;//TODO use chrono for it

public:
	virtual ~IJobOwner();
	bool checkIfStopRequested();
	void playWrapper();
	void onInit();
	void onDeinit();
	void onPlay();
	void onStop();
	EExecState getExecutionState();

	// User-provided methods which user should implement in separate source file (preferably not in i_jobowner.cpp )
	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual EPlayResult play() = 0;


//Exceptions
	class ExJobOwner : public ExEx {
		std::string exInfo;
	public:
		ExJobOwner(const std::string& par_exInfo);

		std::string getInfo() const override;
	};
};

#endif 
