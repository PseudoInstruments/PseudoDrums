#include "Synth.h"

SynthSettings SETTINGS;
Synth SYNTH[CH];

//--------------------------------------------------------------
void SynthSettings::setup() {
	load_json();
	setup_sound_to_color();
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
void SynthSettings::setup_sound_to_color()
{
	sound_to_color_.resize(256);
	// fill -127..127 -> db 0..255
	// for negative sound will return 0
	for (int sound = 0; sound <= 127; sound++) {
		float sndf = util::mapf(sound,
			SETTINGS.vol0, SETTINGS.vol1,
			SETTINGS.amp_from_db0, SETTINGS.amp_from_db1);

		int v = int(util::mapf(util::amp_to_db(sndf),
			SETTINGS.db0, SETTINGS.db1, 0, 255));

		 sound_to_color_[sound + 128] = util::clampi(v, 0, 255);
	}
}

//--------------------------------------------------------------
uint8 SynthSettings::sound_to_color(int8 sound) {	// -127..127 -> db 0..255
	return sound_to_color_[int(sound) + 128];
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
	if (Vol->changed()
		|| SR->changed()
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
	// Duration and sample rate
	int duration_ms = util::mapi_clamp(Duration->value, pot_min, pot_max,
		SETTINGS.duration_ms0, SETTINGS.duration_ms1);

	float sample_rate_note = util::mapf(SR->value, pot_min, pot_max,
		SETTINGS.sample_rate_note0, SETTINGS.sample_rate_note1);
	sample_rate_ = util::note_to_hz_float(sample_rate_note);

	wave_n_ = (long long)(sample_rate_)*duration_ms / 1000;

	// Frequency
	const float square_note0 = 34;   //34 -> 58 Hz
	const float square_note1 = 84;  //84 -> 1046,  72 -> 523 Hz
	const float square_delta = square_note1 - square_note0;

	float note0 = util::mapf_clamp(Freq->value, pot_min, pot_max, square_note0, square_note1);
	float note1 = //util::clampf(
		note0 + util::mapf(FreqDelta->value,
			pot_min, pot_max, -square_delta, +square_delta);

	// Noise, 0 - tone, 127 - noise
	const int noise_max = 127;
	int timbre_noise = util::mapi_clamp(Noise->value, pot_min, pot_max,
		0, noise_max);
	int timbre_tone = noise_max - timbre_noise;

	// Volume and release
	float vol_db = util::mapf(Vol->value, pot_min, pot_max, SETTINGS.db0, SETTINGS.db1);

	int release_start_n = util::mapi(Release->value, pot_min, pot_max,
		wave_n_, 0);

	// Rendering sound
	//float phase = 0;
	//float phase_adder;    //period 1 - <0.5->0, >=0.5->1
	int phase = 0;
	int phase_adder;    //period sample_rate_: 0..sample_rate/2 -> 0, else -> 1
	int sample_rate2 = sample_rate_ / 2;

	int Sound;
	for (int i = 0; i < wave_n_; i++) {
		// Frequency
		float ton = util::mapf(i, 0, wave_n_ - 1, note0, note1);
		int freq = util::note_to_hz_int(ton);
		//phase_adder = float(freq) / sample_rate_;
		phase_adder = freq;

		Sound = timbre_tone * ((phase < sample_rate2) ? -1 : 1);
		phase += phase_adder;
		phase %= sample_rate_;

		// Noise
		Sound += util::randomi(-timbre_noise, timbre_noise);

		Sound = (Sound > 0) ? 1 : -1; //0;   //Sound -1 | 1

		// Volume and release
		float vol_db_momentary = util::mapf_clamp(i, release_start_n, wave_n_,
			vol_db, SETTINGS.db0);

		int volume = int(util::mapf(util::db_to_amp(vol_db_momentary),
			SETTINGS.amp_from_db0, SETTINGS.amp_from_db1,
			SETTINGS.vol0, SETTINGS.vol1));

		Sound *= volume;	// Sound -127..127

		wavebuf_[i] = Sound;
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
void Synth::render_to_image(unsigned char* image_grayscale, int wraw, int hraw,
	int x0raw, int y0raw, int w0raw, int h0raw)
{
	const int pixsize = 2;
	int w = wraw / pixsize;
	int h = hraw / pixsize;
	int x0 = x0raw / pixsize;
	int y0 = y0raw / pixsize;
	int w0 = w0raw / pixsize;
	int h0 = h0raw / pixsize;

	int n0 = w0 * h0;
	int k = 0;
	for (int y = 0; y < h0; y++) {
		for (int x = 0; x < w0; x++) {
			int i = (long long)(k * wave_n_ / n0);
			k++;
			int v = wavebuf_[i];  // -127..127
			//v = (v > 0) ? v * 2 : 0;	// 0..254
			v = SETTINGS.sound_to_color(v);	//0..255

			// Drawpixel as pixsize x pixsize
			int colors[4] = {
				v > 0 ? 255 : 0, v > 128 ? 255 : 0,
				v > 196 ? 255 : 0, v > 64 ? 255 : 0
			};
			int xraw = x0raw + x * pixsize;
			int yraw = y0raw + y * pixsize;
			int k = 0;
			for (int b = 0; b < pixsize; b++) {
				for (int a = 0; a < pixsize; a++) {
					image_grayscale[xraw + a + wraw * (yraw + b)] = colors[k++];
				}
			}
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
			float v = wavebuf_[pos_internal] / 127.f;	// Convert to -1..1
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
