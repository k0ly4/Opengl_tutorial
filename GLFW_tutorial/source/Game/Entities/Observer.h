
#include <Math/Math.h>

class _obs_event {
public:
	enum ChunkHandleEnum :int
	{
		initChunkLight = 0,
		solveLight = 1,
	};
private:
	_obs_event() {}
	~_obs_event() {}

};

///aObserver---------------------------------------------
/// <summary>
///  
/// </summary>
class aObserver {
public:
	virtual void handleEvent(int value) = 0;
private:

};

///Region---------------------------------------------
/// <summary>
///  
/// </summary>
class aListener {
public:

	inline void pushObserver(aObserver* observer) {
		observers.push_back(observer);
	}

	inline void notify(int event) {
		for (size_t i = 0; i < observers.size(); i++)
			observers[i]->handleEvent(event);
	}

private:

	std::vector<aObserver*>observers;
};

///aObserver---------------------------------------------
/// <summary>
///  
/// </summary>
template<typename T>
class uniObserver {
public:

	virtual void handleEvent(int event, T* value) = 0;

private:

};

///Region---------------------------------------------
/// <summary>
///  
/// </summary>
template<typename T>
class uniListener {
public:

	inline void pushObserver(uniObserver<T>* observer) {
		observers.push_back(observer);
	}

	inline void notify(int event, T* value) {
		for (size_t i = 0; i < observers.size(); i++)
			observers[i]->handleEvent(event,value);
	}

private:

	std::vector< uniObserver<T> *>observers;
};