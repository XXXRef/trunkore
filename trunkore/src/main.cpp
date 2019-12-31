#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>

#include "jobsplatform.hpp"
#include "types.hpp"

int main(){
	CJobsPlatform jobsPlatform;

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
	std::string userInputString;
	while (true) {
		std::cout << "> ";
		std::getline(std::cin, userInputString);
		std::stringstream strStream(userInputString);
		std::string firstWord;
		strStream >> firstWord;
		if (firstWord =="EXIT") {
			// Format: "EXIT"

			break;
		}
		else if (firstWord == "LS") {
			// Format: "LS"
			auto jobsNames = jobsPlatform.getJobsNames();
			for (const auto& e : jobsNames) {

				std::cout << "\t" << std::setw(15) << std::left << e << ": " << statesNames[jobsPlatform.getJobState(e)] << std::endl;
			}
		}
		else if (firstWord == "STATE") {
			// Format: "STATE jobName"
			std::string jobNameStr;
			strStream >> jobNameStr;
			try {
				std::cout << "\t" << std::setw(15) << std::left << jobNameStr << ": " << statesNames[jobsPlatform.getJobState(jobNameStr)] << std::endl;
			}
			catch (...) {
				std::cout << "\t" << "Error: no job with given name found" << std::endl;
			}
		}
		else if (firstWord == "ADD") {
			// Format: "ADD jobName jobLibPath jobConfigPath"
			std::string jobNameStr;
			std::string jobLibPathStr;
			std::string jobConfigPathStr;
			strStream >> jobNameStr;
			strStream >> jobLibPathStr;
			strStream >> jobConfigPathStr;
			jobsPlatform.addJob(jobNameStr, jobLibPathStr, jobConfigPathStr);
		}
		else if (firstWord == "RM") {
			// Format: "RM jobName"
			std::string jobNameStr;
			strStream >> jobNameStr;
			jobsPlatform.removeJob(jobNameStr);
		}
		else if (firstWord == "INIT") {
			// Format: "INIT jobName"
			std::string jobNameStr;
			strStream >> jobNameStr;
			try{
				jobsPlatform.initJob(jobNameStr);
			}
			catch (CJobsPlatform::ExNoJobFound& e) {
				std::cout << '\t' << "Error: no job with given name found" << std::endl;
			}
		}
		else if (firstWord == "DEINIT") {
			// Format: "INIT jobName"
			std::string jobNameStr;
			strStream >> jobNameStr;
			try {
				jobsPlatform.deinitJob(jobNameStr);
			}
			catch (CJobsPlatform::ExNoJobFound& e) {
				std::cout << '\t' << "Error: no job with given name found" << std::endl;
			}
		}
		else if (firstWord == "PLAY") {
			// Format: "PLAY jobName"
			std::string jobNameStr;
			strStream >> jobNameStr;
			try {
				jobsPlatform.playJob(jobNameStr);
			}
			catch (CJobsPlatform::ExNoJobFound& e) {
				std::cout << '\t' << "Error: no job with given name found" << std::endl;
			}
		}
		else if (firstWord == "STOP") {
			// Format: "STOP jobName"
			std::string jobNameStr;
			strStream >> jobNameStr;
			try {
				jobsPlatform.stopJob(jobNameStr);
			}
			catch (CJobsPlatform::ExNoJobFound& e) {
				std::cout << '\t' << "Error: no job with given name found" << std::endl;
			}
		}
		else {
			std::cout << '\t' << "Error: unknown command" << std::endl;
		}
	}
	system("pause");
	return EXIT_SUCCESS;
}
