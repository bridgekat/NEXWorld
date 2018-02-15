#ifndef UPDATECOUNTER_H_
#define UPDATECOUNTER_H_

using UpdateCount = unsigned long long;

// GLOBAL GAME TIME in updates(ticks)
class UpdateCounter {
public:
	static void init() { mCounter = 1; }
	static void increase() { mCounter++; }
	static UpdateCount curr() { return mCounter; }

private:
	static UpdateCount mCounter;
};

#endif
