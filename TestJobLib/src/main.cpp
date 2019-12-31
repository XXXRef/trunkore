#include "i_jobowner.hpp"
#include "jobowner.hpp"

extern "C"{
	__declspec(dllexport) IJobOwner* getJob(const std::string& jobConfigFilePath){
		return new CJobOwner(jobConfigFilePath);
	}
}