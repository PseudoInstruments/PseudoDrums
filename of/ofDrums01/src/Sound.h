#pragma once

#include "ofMain.h"
#include "ofxDeSynthUI.h"

class Sound {
public:
	static void create(DeUI::UI* UI);
	static Sound* sound();

	void setup(DeUI::UI* UI);
	void exit();
	void update();

	void audioOut(ofSoundBuffer& output);

protected:
	DeUI::UI* UI_ = nullptr;
	ofSoundStream sound_stream_;
	void setup_sound_stream();

};


