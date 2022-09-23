#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <stdexcept>
class Exception :protected std::exception {
public:
	Exception(const char* message) :std::exception(message) {}
	inline void log()const {
		printf(what());
	}
};
class FramebufferNonCompleteException:public Exception
{	
public:

	FramebufferNonCompleteException() :Exception("(!)Error::FrameBuffer::Non complete\n") {}

};

#endif

