#pragma once

#include "ofMain.h"

struct SynthSettings {
	int sr = 22050;
	int buffer_size = 128;
	int num_buffers = 4;
	void setup();
	void load_json();
};

class Synth {
public:
	void setup();

protected:

};

extern SynthSettings SETTINGS;

const int CH = 4;
extern Synth SYNTH[CH];

