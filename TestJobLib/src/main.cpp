#include "EXT/trunkore/inc/i_jobowner.hpp"
#include "jobowner.hpp"

extern "C"{
	__declspec(dllexport) IJobOwner* getJob(const LPCWSTR &jobConfigFilePath){
		return new CJobOwner(jobConfigFilePath);
	}
}