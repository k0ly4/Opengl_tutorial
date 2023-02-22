#ifndef THREAD_H
#define THREAD_H
#include <thread>
#include <atomic>
#include "Math/GLM.h"




class Thread
{
	
public:

	enum Status: byte
	{
		sDeath, sWork, sWait
	};
	enum Command : byte
	{
		toDeath, toWork, toWait
	};
	template<typename T>
	void launch(T* obj, void (T::* f)()) {
		if (status == sWork) kill();
		command = toWork;
		thread = new std::thread([obj,f,this]{
				while (this->command != toDeath) {
					if (this->command == toWork) {
						this->status = sWork;
						(obj->*f)();
					}
					else if (this->command == toWait) {
						this->status = sWait;
						while (this->command == toWait) std::this_thread::yield();
					}
				}
			}
		);
	}

	inline void kill() noexcept {
		if (status != sDeath) {
			command = toDeath;
			thread->join();
			delete thread;
			status = sDeath;
		}
	}
	
	~Thread() {
		kill();
	}

	
	std::atomic<byte> command = toDeath;
	std::atomic<byte> status = sDeath;

private:
	

	std::thread* thread;

};

#endif THREAD_H
