#pragma once

#include "ofMain.h"

class Sound {
public:
	static void create();
	static Sound* sound();

	void setup();
	void exit();
	void update();

	void audioOut(ofSoundBuffer& output);

protected:
	ofSoundStream sound_stream_;
	void setup_sound_stream();

};


