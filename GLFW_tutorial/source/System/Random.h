#ifndef RANDOM_H
#define RANDOM_H
#include "Math\Entities.h"

/////qRandom----------------------------------
class qRandom {

public:

	qRandom() { seed = 1676827905; }
	qRandom(size_t seed_) :seed(seed_) {}

	inline int rand()noexcept {
		seed = (8253729 * seed + 2396403);
		return seed % 32768;
	}
	inline int get(int max)noexcept { return (rand() % max); }
	inline int get(const Range<int>& range)noexcept { return (rand() % (range.max - range.min)) + range.min; }
	inline void setSeed(size_t number)noexcept {
		seed = number + 8253729;
		rand();
	}

private:

	size_t seed;

};

/////sQrand----------------------------------
class sQrand {
public:

	inline static int get()noexcept { return random.rand(); }
	inline static int get(int max)noexcept { return (random.rand() % max); }
	inline static int get(const Range<int>& range)noexcept { return (random.rand() % (range.max - range.min)) + range.min; }

private:

	sQrand() {}
	~sQrand() {}
	static qRandom random;
};
#endif