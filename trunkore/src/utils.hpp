#ifndef HEADER_UTILS
#define HEADER_UTILS

#include <string>
#include <exception>

//====================================================================================================
class ExEx : public std::exception {

public:
	ExEx() = default; //Not necessary: for now its the only constructor
	virtual ~ExEx() {}
	virtual std::string getInfo() const = 0;
};

//TODO add operator<<(std::ostream&, const ExEx &)

#endif