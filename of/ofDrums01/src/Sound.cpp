#include "Sound.h"
#include "DeTypesAssert.h"
#include "ofxSoundUtils.h"

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
	//sound_stream_.printDeviceList();
	auto devices = sound_stream_.getDeviceList(ofSoundDevice::Api::MS_DS);
	cout << devices << endl;
	de_assert(!devices.empty(), "No sound devices");

	ofSoundStreamSettings settings;
	//settings.setInDevice(devices[0]);
	settings.setOutDevice(devices[0]);

	// settings.api = ofSoundDevice::Api::PULSE;
	//auto devices = sound_stream_.getMatchingDevices("default");


	stream_sr_ = 22050;
	int buffer_size = 128;
	int num_buffers = 4;

	settings.setOutListener(this);
	settings.sampleRate = stream_sr_;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 2;
	settings.bufferSize = buffer_size;
	settings.numBuffers = num_buffers;
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
