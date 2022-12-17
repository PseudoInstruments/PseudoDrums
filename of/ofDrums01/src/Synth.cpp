#include "Synth.h"
#include "DeTypesAssert.h"
#include "ofxSoundUtils.h"

SynthSettings SETTINGS;
Synth SYNTH[CH];

//--------------------------------------------------------------
void SynthSettings::setup() {
	load_json();
}

//--------------------------------------------------------------
void SynthSettings::load_json() {
	ofJson json = ofLoadJson("settings.json");

	sr = json["sr"];
	buffer_size = json["buffer_size"];
	num_buffers = json["num_buffers"];

}


//--------------------------------------------------------------
void Synth::setup(int i, DeUI::UI* UI) {
	i_ = i;
	UI_ = UI;

	Vol = &UI->Vol[i];
	SR = &UI->SR[i];
	Freq = &UI->Freq[i];
	FreqDelta = &UI->FreqDelta[i];
	Duration = &UI->Duration[i];
	Release = &UI->Release[i];
	Timbre = &UI->Timbre[i];
	Noise = &UI->Noise[i];
	Pad = &UI->Pad[i];

}


//--------------------------------------------------------------
