#pragma once

#include "ofMain.h"

class Sound {
public:
	static void create();
	static Sound* sound();

	void setup();
	void exit();
	void update();
	void draw(const ofRectangle& rect_pix);

	void audioOut(ofSoundBuffer& output);

protected:
	int stream_sr_ = 0;
	ofSoundStream sound_stream_;

};


