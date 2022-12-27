#pragma once
#include "ofMain.h"
#include <atomic>

// Thread for reading keyboard and Arduino input
class ControlThread : public ofThread
{
public:
	~ControlThread() {
		stop();
		waitForThread(false);
	}

	void start() {
		startThread();
	}

	void stop() {
		stopThread();
	}

	void threadedFunction() {
		while (isThreadRunning()) {

		}
	}

protected:
};
