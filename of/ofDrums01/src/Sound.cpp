#include "Sound.h"
#include "DeTypesAssert.h"
#include "ofxSoundUtils.h"
#include "Synth.h"

Sound* g_Sound = nullptr;

//--------------------------------------------------------------
void Sound::create(DeUI::UI* UI) {
	g_Sound = new Sound;
	g_Sound->setup(UI);
}

//--------------------------------------------------------------
Sound* Sound::sound() {
	return g_Sound;
}

//--------------------------------------------------------------
void Sound::setup(DeUI::UI* UI) {
	UI_ = UI;
	for (int i = 0; i < CH; i++) {
		SYNTH[i].setup(i, UI);
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

	// Api: 
	// DEFAULT
	// MS_WASAPI /*!< The Microsoft WASAPI API. */
	// MS_ASIO   /*!< The Steinberg Audio Stream I/O API. */
	// MS_DS     /*!< The Microsoft Direct Sound API. */

	ofSoundStreamSettings settings;
	settings.setApi(ofSoundDevice::Api::MS_WASAPI);
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
void Sound::init_waves()
{
	for (int i = 0; i < CH; i++) {
		SYNTH[i].init_wave();
	}
}

//--------------------------------------------------------------
void Sound::audioOut(ofSoundBuffer& output) {
	auto& buffer = output.getBuffer();
	if (buffer.empty()) return;

	float* data = buffer.data();
	int n = buffer.size();
	// expected 2 channels
	de_assert(output.getNumChannels() == 2, "Sound::audioOut - expected stereo buffer to fill");
	int nframes = n / output.getNumChannels();

	for (int i = 0; i < CH; i++) {
		SYNTH[i].audio_add_stereo(data, nframes);
	}

	// Apply compression and final volume
	float maxv = CH;
	float Volume = UI_->value_to_float(UI_->Volume.value, FaderMax);
	for (int k = 0; k < n; k++) {
		float& v = data[k];
		v *= 1.0f / CH;	// 0..1		//TODO make compression
		v *= Volume;
	}

}

//--------------------------------------------------------------
void Sound::update() {
	for (int i = 0; i < CH; i++) {
		SYNTH[i].update();
	}

}

//--------------------------------------------------------------
