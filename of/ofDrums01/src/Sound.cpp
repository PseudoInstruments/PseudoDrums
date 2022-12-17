#include "Sound.h"
#include "DeTypesAssert.h"
#include "ofxSoundUtils.h"
#include "Synth.h"

Sound* g_Sound = nullptr;

//--------------------------------------------------------------
void Sound::create() {
	g_Sound = new Sound;
	g_Sound->setup();
}

//--------------------------------------------------------------
Sound* Sound::sound() {
	return g_Sound;
}

//--------------------------------------------------------------
void Sound::setup() {
	SETTINGS.setup();
	for (int i = 0; i < CH; i++) {
		SYNTH[i].setup();
	}
	setup_sound_stream();

}

//--------------------------------------------------------------
void Sound::setup_sound_stream()
{
	//sound_stream_.printDeviceList();
	auto devices = sound_stream_.getDeviceList(ofSoundDevice::Api::MS_DS);
	cout << devices << endl;
	de_assert(!devices.empty(), "No sound devices");

	ofSoundStreamSettings settings;
	//settings.setInDevice(devices[0]);
	settings.setOutDevice(devices[0]);

	// settings.api = ofSoundDevice::Api::PULSE;
	//auto devices = sound_stream_.getMatchingDevices("default");

	settings.setOutListener(this);
	settings.sampleRate = SETTINGS.sr;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 2;
	settings.bufferSize = SETTINGS.buffer_size;
	settings.numBuffers = SETTINGS.num_buffers;
	sound_stream_.setup(settings);
}

//--------------------------------------------------------------
void Sound::exit() {
	sound_stream_.close();
}

//--------------------------------------------------------------
void Sound::audioOut(ofSoundBuffer& output) {
	const float volume = 0.6f;
	for (int i = 0; i < output.getNumFrames(); i++) {
		output.getSample(i, 0) = 0;
		output.getSample(i, 1) = 0;

	}
}

//--------------------------------------------------------------
void Sound::update() {


}

//--------------------------------------------------------------
