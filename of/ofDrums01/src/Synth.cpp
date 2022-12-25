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

	sr = json["sr"];
#define LOADVAL(NAME) NAME = json[#NAME]
	LOADVAL(buffer_size);
	LOADVAL(num_buffers);
	LOADVAL(sample_rate_note0);
	LOADVAL(sample_rate_note1);
#undef LOADVAL
}

//--------------------------------------------------------------
void Synth::setup(int i, DeUI::UI* UI) {
	i_ = i;
	UI_ = UI;

	pot_min = 0;
	pot_max = FaderMax;

	Vol = &UI->Vol[i];
	SR = &UI->SR[i];
	Freq = &UI->Freq[i];
	FreqDelta = &UI->FreqDelta[i];
	Duration = &UI->Duration[i];
	Release = &UI->Release[i];
	Timbre = &UI->Timbre[i];
	Noise = &UI->Noise[i];
	Pad = &UI->Pad[i];

	init_wave();
}

//--------------------------------------------------------------
void Synth::init_wave() {
	const float duration_ms0 = 10;
	const float duration_ms1 = 500;
	int duration_ms = util::mapi_clamp(*Duration, pot_min, pot_max, duration_ms0, duration_ms1);

	const float square_note0 = 34;   //34 -> 58 Hz
	const float square_note1 = 84;  //84 -> 1046,  72 -> 523 Hz
	const float square_delta = square_note1 - square_note0;

	float note0 = util::mapf_clamp(*Freq, pot_min, pot_max, square_note0, square_note1);
	float note1 = util::clampf(note0 + util::mapf(*FreqDelta,
		pot_min, pot_max, -square_delta, +square_delta),
		square_note0, square_note1);

	// noise, 0 - tone, 127 - noise
	int timbre_noise = util::mapi_clamp(*Noise, pot_min, pot_max, 0, 127);
	int timbre_tone = 127 - timbre_noise;

	// sample rate
	float sample_rate_note = util::mapf(*SR, pot_min, pot_max, SETTINGS.sample_rate_note0, SETTINGS.sample_rate_note1);
	sample_rate_ = util::note_to_hz_float(sample_rate_note);

	wave_n_ = (long long)(sample_rate_)*duration_ms / 1000;
	wavebuf_.resize(wave_n_);

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
			+ random(-timbre_noise, timbre_noise);
		wavebuf_[i] = (sound > 0) ? 1 : 0;
		phase += phase_adder;
		phase %= sample_rate_;
	}
	
		//Debug print
		//Serial.println("Drum update");
		//Serial.print("duration_ms "); Serial.println(duration_ms);
		//Serial.print("wave_n "); Serial.println(wave_n);
		//Serial.print("Freq "); Serial.print(m_to_f_int(note0)); Serial.print(" -- "); Serial.println(m_to_f_int(note1));
		//Serial.print("Timbre noise "); Serial.println(timbre_noise);

		/*for (int i=0; i<wave_n; i++) {
		  Serial.print(wave[i]); Serial.print(" ");
		  if (i % 40 == 0 && i > 0) Serial.println();
		}
		Serial.println(); */

		//Serial.println("MIDI to freq:");
		//for (int i=0; i<127; i++) {
		//  Serial.print(i); Serial.print(" ");Serial.println(m_to_f_int(i)); 
		//}
}

//--------------------------------------------------------------
void Synth::audio_add_stereo(float* data, int nframes) {

}

//--------------------------------------------------------------
