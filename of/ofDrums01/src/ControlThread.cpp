#include "ControlThread.h"
#include "DeTypesAssert.h"
#include "Sound.h"

//--------------------------------------------------------------
ControlThread::~ControlThread() {
	stop();
}

//--------------------------------------------------------------
void ControlThread::start(DeUI::UI* UI, int rate) {
	UI_ = UI;
	delay_ = 1000 / rate;
	startThread();

	key_states_.resize(255);
}

//--------------------------------------------------------------
void ControlThread::stop() {
	stopThread();
	const int wait_ms = 100;
	waitForThread(false, wait_ms);
}

//--------------------------------------------------------------
void ControlThread::threadedFunction() {
	while (isThreadRunning()) {
		// Receive control events
		process_keyboard();

		// Update UI and sound
		UI_->update();
		Sound::sound()->update();
		UI_->store_last_values();

		ofSleepMillis(delay_);
	}
}

//--------------------------------------------------------------
void ControlThread::process_key(int key, int VK) {
	int8 new_state = (GetAsyncKeyState(VK) != 0);
	if (new_state != key_states_[key]) {
		key_states_[key] = new_state;
		if (new_state) {
			UI_->keyPressed(key);
		}
		else {
			UI_->keyReleased(key);
		}
	}
}

//--------------------------------------------------------------
void ControlThread::process_keyboard() {
	//Virtual key codes: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	// Send only numbers
	for (int key = '0'; key <= '9'; key++) {
		process_key(key, key);
	}
}

//--------------------------------------------------------------
