#include "Synth.h"
#include "DeTypesAssert.h"

SynthSettings SETTINGS;
Synth SYNTH[CH];

//--------------------------------------------------------------
void SynthSettings::setup() {
	load_json();
}

//--------------------------------------------------------------
void SynthSettings::load_json() {
	ofJson json = ofLoadJson("settings.json");

#define LOADVAL(NAME) NAME = json[#NAME]
	LOADVAL(control_rate);
	LOADVAL(sr);
	LOADVAL(buffer_size);
	LOADVAL(num_buffers);
	LOADVAL(sample_rate_note0);
	LOADVAL(sample_rate_note1);
	LOADVAL(duration_ms0);
	LOADVAL(duration_ms1);

#undef LOADVAL
}

//--------------------------------------------------------------
int SynthSettings::max_samples() const {
	int max_sample_rate = util::note_to_hz_float(sample_rate_note1) + 1;
	int max_duration = duration_ms1;
	return max_sample_rate * max_duration;
}

//--------------------------------------------------------------
void Synth::setup(int id, DeUI::UI* UI) {
	id_ = id;
	UI_ = UI;

	pot_min = 0;
	pot_max = FaderMax;

	Vol = &UI->Vol[id];
	SR = &UI->SR[id];
	Freq = &UI->Freq[id];
	FreqDelta = &UI->FreqDelta[id];
	Duration = &UI->Duration[id];
	Release = &UI->Release[id];
	Timbre = &UI->Timbre[id];
	Noise = &UI->Noise[id];
	Pad = &UI->Pad[id];

	wavebuf_.resize(SETTINGS.max_samples());

	// Wave
	init_wave();

}

//--------------------------------------------------------------
void Synth::update() {
	if (SR->changed()
		|| Freq->changed()
		|| FreqDelta->changed()
		|| Duration->changed()
		|| Release->changed()
		|| Timbre->changed()
		|| Noise->changed()) {
		init_wave();
	}

	if (Pad->changed() && Pad->value) {
		play_wave();
	}
}

//--------------------------------------------------------------
void Synth::init_wave() {
	int duration_ms = util::mapi_clamp(Duration->value, pot_min, pot_max,
		SETTINGS.duration_ms0, SETTINGS.duration_ms1);

	const float square_note0 = 34;   //34 -> 58 Hz
	const float square_note1 = 84;  //84 -> 1046,  72 -> 523 Hz
	const float square_delta = square_note1 - square_note0;

	float note0 = util::mapf_clamp(Freq->value, pot_min, pot_max, square_note0, square_note1);
	float note1 = //util::clampf(
		note0 + util::mapf(FreqDelta->value,
			pot_min, pot_max, -square_delta, +square_delta);
	//,square_note0, square_note1);

// noise, 0 - tone, 127 - noise
	int timbre_noise = util::mapi_clamp(Noise->value, pot_min, pot_max, 0, 127);
	int timbre_tone = 127 - timbre_noise;

	// sample rate
	float sample_rate_note = util::mapf(SR->value, pot_min, pot_max, SETTINGS.sample_rate_note0, SETTINGS.sample_rate_note1);
	sample_rate_ = util::note_to_hz_float(sample_rate_note);

	wave_n_ = (long long)(sample_rate_)*duration_ms / 1000;

	//float phase = 0;
	//float phase_adder;    //period 1 - <0.5->0, >=0.5->1
	int phase = 0;
	int phase_adder;    //period sample_rate_: 0..sample_rate/2 -> 0, else -> 1
	int sample_rate2 = sample_rate_ / 2;

	for (int i = 0; i < wave_n_; i++) {
		float ton = util::mapf(i, 0, wave_n_ - 1, note0, note1);
		int freq = util::note_to_hz_int(ton);
		//phase_adder = float(freq) / sample_rate_;
		phase_adder = freq;

		int sound = timbre_tone * ((phase < sample_rate2) ? -1 : 1)
			+ util::randomi(-timbre_noise, timbre_noise);
		wavebuf_[i] = (sound > 0) ? 1 : 0;
		phase += phase_adder;
		phase %= sample_rate_;
	}

	is_changed_ = true;
	//Debug print

/*	cout << "Drum " << id_ + 1 << " update" << endl;
	cout << "duration_ms " << duration_ms << endl;
	cout << "wave_n " << wave_n_ << endl;
	cout << "Freq " << util::note_to_hz_int(note0)
		<< " -- " << util::note_to_hz_int(note1) << endl;
	cout << "Noise " << timbre_noise << endl;;

	for (int i = 0; i < wave_n_; i++) {
		if (i % 80 == 0 && i > 0) cout << endl;
		cout << int(wavebuf_[i]);
	}
	cout << endl;
	*/
}

//--------------------------------------------------------------
void Synth::play_wave()
{
	playing_ = 1;
	play_pos_external_ = 0;

}

//--------------------------------------------------------------
void Synth::render_to_image(unsigned char* image_grayscale, int w, int h,
	int x0, int y0, int w0, int h0)
{
	int n0 = w0 * h0;
	int k = 0;
	for (int y = 0; y < h0; y++) {
		for (int x = 0; x < w0; x++) {
			int i = (long long)(k * wave_n_ / n0);
			k++;
			image_grayscale[x0 + x + w * (y0 + y)] = (wavebuf_[i] > 0) ? 255 : 0;
		}
	}
}

//--------------------------------------------------------------
void Synth::audio_add_stereo(float* data, int nframes) {
	if (playing_) {
		int sr_external = SETTINGS.sr;

		int k = 0;
		for (int i = 0; i < nframes; i++) {
			int pos_internal = (long long)sample_rate_ * play_pos_external_ / sr_external;
			if (pos_internal >= wave_n_) {
				playing_ = 0;
				break;
			}
			float v = wavebuf_[pos_internal];
			data[k++] += v;
			data[k++] += v;
			play_pos_external_++;
		}
	}
}

//--------------------------------------------------------------
bool Synth::is_changed() {
	bool result = is_changed_;
	is_changed_ = false;
	return result;
}

//--------------------------------------------------------------
