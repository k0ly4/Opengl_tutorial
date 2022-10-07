#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>
////LOG-----------------------------------------------
enum TypeLog:size_t {
	LogError,
	LogWarning,
	LogInfo,
};

static const char* LOG_attribute[] = {"(!)ERROR::","WARNING::","INFO::"};

inline void LOG(TypeLog typeLog,
	const char* strFormat, ...)noexcept
{
	printf("%s%s", LOG_attribute[typeLog],strFormat);
}


inline void LOG( const char* strFormat, ...)noexcept
{
	printf("%s%s", LOG_attribute[LogInfo], strFormat);
}

inline void LOG(const std::string& notice)
noexcept {
	printf("%s%s", LOG_attribute[LogInfo], notice.c_str());
}

////Exception-----------------------------------------------
/// <summary>
/// 
/// </summary>
class Exception :protected std::exception {
public:
	Exception(const char* message) 
		:std::exception(message) 
	{}
	inline void log()const {
		LOG(what());
	}
};

class FramebufferNonCompleteException:public Exception
{	
public:

	FramebufferNonCompleteException() :Exception("FrameBuffer::Non complete\n") {}

};
class stbiResourceException :public Exception {
public:
	stbiResourceException(const char* reason)
		:Exception(reason){}
};
#endif

