#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <stdexcept>

inline void log(const std::string& message) noexcept {
	printf(message.c_str());
}

class Exception :protected std::exception {
public:
	Exception(const char* message) 
		:std::exception(message) 
	{}
	inline void log()const {
		printf(what());
	}
};

class FramebufferNonCompleteException:public Exception
{	
public:

	FramebufferNonCompleteException() :Exception("(!)Error::FrameBuffer::Non complete\n") {}

};
class stbiResourceException :public Exception {
public:
	stbiResourceException(const char* reason)
		:Exception(reason){}
};
#endif

