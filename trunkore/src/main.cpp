#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>

#include "jobsplatform.hpp"
#include "types.hpp"

#include "config.hpp"

#if defined PLATFORM_WINDOWS
/*
	using MAINCFG_STRTYPE=std::wstring;
	#define MAINCFG_STD_IN_OBJ std::wcin
	#define MAINCFG_STD_OUT_OBJ std::wcout
	using STRSTREAM=std::wstringstream;
	//Commands
	#define MAINCFG_CMD_EXIT L"EXIT"
	#define MAINCFG_CMD_LS L"LS"
	#define MAINCFG_CMD_STATE L"STATE"
	#define MAINCFG_CMD_ADD L"ADD"
	#define MAINCFG_CMD_RM L"RM"
	#define MAINCFG_CMD_INIT L"INIT"
	#define MAINCFG_CMD_DEINIT L"DEINIT"
	#define MAINCFG_CMD_PLAY L"PLAY"
	#define MAINCFG_CMD_STOP L"STOP"
*/
using MAINCFG_STRTYPE=std::string;
#define MAINCFG_STD_IN_OBJ std::cin
#define MAINCFG_STD_OUT_OBJ std::cout
using STRSTREAM=std::stringstream;
//Commands
#define MAINCFG_CMD_EXIT "EXIT"
#define MAINCFG_CMD_LS "LS"
#define MAINCFG_CMD_STATE "STATE"
#define MAINCFG_CMD_ADD "ADD"
#define MAINCFG_CMD_RM "RM"
#define MAINCFG_CMD_INIT "INIT"
#define MAINCFG_CMD_DEINIT "DEINIT"
#define MAINCFG_CMD_PLAY "PLAY"
#define MAINCFG_CMD_STOP "STOP"

#elif defined PLATFORM_NIX
using MAINCFG_STRTYPE=std::string;
#define MAINCFG_STD_IN_OBJ std::cin
#define MAINCFG_STD_OUT_OBJ std::cout
using STRSTREAM=std::stringstream;
//Commands
#define MAINCFG_CMD_EXIT "EXIT"
#define MAINCFG_CMD_LS "LS"
#define MAINCFG_CMD_STATE "STATE"
#define MAINCFG_CMD_ADD "ADD"
#define MAINCFG_CMD_RM "RM"
#define MAINCFG_CMD_INIT "INIT"
#define MAINCFG_CMD_DEINIT "DEINIT"
#define MAINCFG_CMD_PLAY "PLAY"
#define MAINCFG_CMD_STOP "STOP"
/*
//States
#define MAINCFG_STATE_VANILLA "VANILLA"
#define MAINCFG_STATE_INITIALIZED "INITIALIZED"
#define MAINCFG_STATE_PLAYING "PLAYING"
#define MAINCFG_STATE_STOPPED "STOPPED"
#define MAINCFG_STATE_ENDED "ENDED"
#define MAINCFG_STATE_DEINITIALIZED "DEINITIALIZED"
#define MAINCFG_STATE_INTERRUPTED "INTERRUPTED"
*/
#endif

int main() {
	CJobsPlatform jobsPlatform;
	/*
	std::map<IJobOwner::EExecState, MAINCFG_STRTYPE> statesNames{
		{ IJobOwner::EExecState::VANILLA,MAINCFG_STATE_VANILLA },
		{ IJobOwner::EExecState::INITIALIZED,MAINCFG_STATE_INITIALIZED },
		{ IJobOwner::EExecState::PLAYING,MAINCFG_STATE_PLAYING },
		{ IJobOwner::EExecState::STOPPED,MAINCFG_STATE_STOPPED },
		{ IJobOwner::EExecState::ENDED,MAINCFG_STATE_ENDED },
		{ IJobOwner::EExecState::DEINITIALIZED,MAINCFG_STATE_DEINITIALIZED },
		{ IJobOwner::EExecState::INTERRUPTED,MAINCFG_STATE_INTERRUPTED }
	};
	*/
	std::map<IJobOwner::EExecState, std::string> statesNames{
		{ IJobOwner::EExecState::VANILLA,"VANILLA" },
		{ IJobOwner::EExecState::INITIALIZED,"INITIALIZED" },
		{ IJobOwner::EExecState::PLAYING,"PLAYING" },
		{ IJobOwner::EExecState::STOPPED,"STOPPED" },
		{ IJobOwner::EExecState::ENDED,"ENDED" },
		{ IJobOwner::EExecState::DEINITIALIZED,"DEINITIALIZED" },
		{ IJobOwner::EExecState::INTERRUPTED,"INTERRUPTED" }
	};
	// Handle user input
	MAINCFG_STRTYPE userInputString;
	while (true) {
		std::cout << "> ";//TIP we can mix std::cout and std::wcout usage
		std::getline(MAINCFG_STD_IN_OBJ, userInputString);
		STRSTREAM strStream(userInputString);
		MAINCFG_STRTYPE firstWord;
		strStream >> firstWord;
		if (firstWord == MAINCFG_CMD_EXIT) {
			// Format: "EXIT"

			break;
		}
		else if (firstWord == MAINCFG_CMD_LS) {
			// Format: "LS"
			auto jobsIDs = jobsPlatform.getJobsIDs();
			for (const auto& e : jobsIDs) {
				std::cout << "\t";
				MAINCFG_STD_OUT_OBJ << std::setw(15) << std::left << e;
				std::cout << ": " << statesNames[jobsPlatform.getJobState(e)] << std::endl;
			}
		}
		else if (firstWord == MAINCFG_CMD_STATE) {
			// Format: "STATE jobID"
			MAINCFG_STRTYPE jobIDStr;
			strStream >> jobIDStr;
			try {
				std::cout << "\t";
				MAINCFG_STD_OUT_OBJ << std::setw(15) << std::left << jobIDStr << ": ";
				std::cout << statesNames[jobsPlatform.getJobState(jobIDStr)] << std::endl;
			}
			catch (...) {
				std::cout << "\t" << "Error: no job with given name found" << std::endl;
			}
		}
		else if (firstWord == MAINCFG_CMD_ADD) {
			// Format: "ADD jobID jobLibPath jobConfigPath"
			MAINCFG_STRTYPE jobIDStr;
			MAINCFG_STRTYPE jobLibPathStr;
			MAINCFG_STRTYPE jobConfigPathStr;
			strStream >> jobIDStr;
			strStream >> jobLibPathStr;
			strStream >> jobConfigPathStr;
			jobsPlatform.addJob(jobIDStr, jobLibPathStr.c_str(), jobConfigPathStr.c_str());
		}
		else if (firstWord == MAINCFG_CMD_RM) {
			// Format: "RM jobID"
			MAINCFG_STRTYPE jobIDStr;
			strStream >> jobIDStr;
			jobsPlatform.removeJob(jobIDStr);
		}
		else if (firstWord == MAINCFG_CMD_INIT) {
			// Format: "INIT jobID"
			MAINCFG_STRTYPE jobIDStr;
			strStream >> jobIDStr;
			try {
				jobsPlatform.initJob(jobIDStr);
			}
			catch (CJobsPlatform::ExJobsPlatform & e) {
				std::cout << '\t' << "[ERROR] " << e.getInfo() << std::endl; //TODO exception info can be in unicode form
			}
		}
		else if (firstWord == MAINCFG_CMD_DEINIT) {
			// Format: "INIT jobID"
			MAINCFG_STRTYPE jobIDStr;
			strStream >> jobIDStr;
			try {
				jobsPlatform.deinitJob(jobIDStr);
			}
			catch (CJobsPlatform::ExJobsPlatform & e) {
				std::cout << '\t' << "[ERROR] " << e.getInfo() << std::endl; //TODO exception info can be in unicode form
			}
		}
		else if (firstWord == MAINCFG_CMD_PLAY) {
			// Format: "PLAY jobID"
			MAINCFG_STRTYPE jobIDStr;
			strStream >> jobIDStr;
			try {
				jobsPlatform.playJob(jobIDStr);
			}
			catch (CJobsPlatform::ExJobsPlatform & e) {
				std::cout << '\t' << "[ERROR] " << e.getInfo() << std::endl; //TODO exception info can be in unicode form
			}
		}
		else if (firstWord == MAINCFG_CMD_STOP) {
			// Format: "STOP jobID"
			MAINCFG_STRTYPE jobIDStr;
			strStream >> jobIDStr;
			try {
				jobsPlatform.stopJob(jobIDStr);
			}
			catch (CJobsPlatform::ExJobsPlatform & e) {
				std::cout << '\t' << "[ERROR] " << e.getInfo() << std::endl;
			}
		}
		else {
			std::cout << '\t' << "[ERROR] unknown command" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
