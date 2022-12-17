#pragma once

#include "ofMain.h"
#include "ofxDeSynthUI.h"

struct SynthSettings {
	int sr = 22050;
	int buffer_size = 128;
	int num_buffers = 4;
	void setup();
	void load_json();
};

class Synth {
public:
	void setup(int i, DeUI::UI *UI);

protected:
	int i_ = 0;
	DeUI::UI* UI_ = nullptr;

	int* Vol = nullptr;
	int* SR = nullptr;
	int* Freq = nullptr;
	int* FreqDelta = nullptr;
	int* Duration = nullptr;
	int* Release = nullptr;
	int* Timbre = nullptr;
	int* Noise = nullptr;
	int* Pad = nullptr;

};

extern SynthSettings SETTINGS;

const int CH = 4;
extern Synth SYNTH[CH];

