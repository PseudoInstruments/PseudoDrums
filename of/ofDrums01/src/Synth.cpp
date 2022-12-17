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
void Synth::setup() {


}


//--------------------------------------------------------------
