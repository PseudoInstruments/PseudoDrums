#pragma once

#include "ofMain.h"
#include "ofxDeSynthUI.h"
#include "ofxSoundUtils.h"
typedef ofxSoundUtils util;


struct SynthSettings {
	int sr = 22050;
	int buffer_size = 128;
	int num_buffers = 4;
	float sample_rate_note0 = 60;   //60 -> 261.63 Hz
	float sample_rate_note1 = 114;  //114 -> 5919.91 Hz

	int duration_ms0 = 10;
	int duration_ms1 = 500;


	int max_samples() const;

	void setup();
	void load_json();
};

class Synth {
public:
	void setup(int id, DeUI::UI *UI);
	void audio_add_stereo(float* data, int nframes);
	void init_wave();

	void play_wave();

protected:
	int id_ = 0;
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

	int pot_min = 0;
	int pot_max = 10;
	
	int sample_rate_ = 1;
	int wave_n_ = 0;
	vector<unsigned char> wavebuf_;

};

extern SynthSettings SETTINGS;

const int CH = 4;
extern Synth SYNTH[CH];

