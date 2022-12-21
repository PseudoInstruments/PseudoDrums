#pragma once

#include "ofMain.h"
#include "ofxDeSynthUI.h"
#include "ofxSoundUtils.h"
typedef ofxSoundUtils util;


struct SynthSettings {
	int sr = 22050;
	int buffer_size = 128;
	int num_buffers = 4;
	float sample_rate_hz0 = 260;
	float sample_rate_hz1 = 6000;

	void setup();
	void load_json();

	// Sample rates
	// Make table of sample rates
	void init_sample_rates();

	//Table of sample rates
	static const int sample_rates_n = FaderMax;
	int sample_rates[sample_rates_n];
};

class Synth {
public:
	void setup(int i, DeUI::UI *UI);
	void audio_add_stereo(float* data, int nframes);

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

	void init_wave();
};

extern SynthSettings SETTINGS;

const int CH = 4;
extern Synth SYNTH[CH];

