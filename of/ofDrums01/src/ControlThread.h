#pragma once
#include "ofMain.h"
#include "ofxDeSynthUI.h"

// Thread for reading keyboard and Arduino input
class ControlThread : public ofThread
{
public:
	~ControlThread();

	// Rate is measured in millis, so may be innacurate
	void start(DeUI::UI* UI, int rate);
	void stop();
	virtual void threadedFunction() override;

protected:
	DeUI::UI* UI_ = nullptr;
	int delay_ = 10;
	void process_keyboard();

	void process_key(int key, int VK);
	vector<int> key_states_;
};
