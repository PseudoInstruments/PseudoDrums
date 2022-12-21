#include "Synth.h"
#include "DeTypesAssert.h"

SynthSettings SETTINGS;
Synth SYNTH[CH];

//--------------------------------------------------------------
void SynthSettings::setup() {
	load_json();
	init_sample_rates();
}

//--------------------------------------------------------------
void SynthSettings::load_json() {
	ofJson json = ofLoadJson("settings.json");

	sr = json["sr"];
#define LOADVAL(NAME) NAME = json[#NAME]
	LOADVAL(buffer_size);
	LOADVAL(num_buffers);
	LOADVAL(sample_rate_hz0);
	LOADVAL(sample_rate_hz1);
#undef LOADVAL
}

//--------------------------------------------------------------
//make table of sample rates
void SynthSettings::init_sample_rates() {
	// We enumerate sample rates in exp scale
	float sample_rate_note0 = util::hz_to_note(sample_rate_hz0);
	float sample_rate_note1 = util::hz_to_note(sample_rate_hz1);
	for (int i = 0; i < sample_rates_n; i++) {
		float note = util::mapf(i, 0, sample_rates_n - 1, sample_rate_note0, sample_rate_note1);
		sample_rates[i] = util::note_to_hz_float(note);
	}

	cout << "Sample rates: " << sample_rates[0] << " - " << sample_rates[sample_rates_n - 1] << endl;

	//for (int i=0; i<sample_rates_n; i++) {
	//  Serial.print(i); Serial.print(" "); Serial.println(sample_rates[i]);
	//}
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
void Synth::init_wave() {
	vector<unsigned char> wavebuf(100000);
	unsigned char* wave = wavebuf.data();	
	int wave_n;

	/*
	int duration_ms = mapi_clamp(duration_slider, pot_min, pot_max, 10, 500);

	float note0 = mapf_clamp(tone1_slider, pot_min, pot_max, square_note0, square_note1);
	float note1 = clampf(note0 + mapf(tone2_slider, pot_min, pot_max, -square_delta, +square_delta), square_note0, square_note1);

	int n_max = (long long)(sample_rate_)*duration_ms / 1000;
	wave_n = min(n_max, wave_N);

	//float phase = 0;
	//float phase_adder;    //period 1 - <0.5->0, >=0.5->1
	int phase = 0;
	int phase_adder;    //period sample_rate_: 0..sample_rate/2 -> 0, else -> 1
	int sample_rate2 = sample_rate_ / 2;

	//timbre, 0 - tone, 127 - noise
	int timbre_noise = mapi_clamp(timbre_slider, pot_min, pot_max, 0, 127);
	int timbre_tone = 127 - timbre_noise;

	for (int i = 0; i < wave_n; i++) {
		float ton = mapf(i, 0, wave_n - 1, note0, note1);
		int freq = m_to_f_int(ton);
		//phase_adder = float(freq) / sample_rate_;
		phase_adder = freq;

		int sound = timbre_tone * ((phase < sample_rate2) ? -1 : 1) + random(-timbre_noise, timbre_noise);
		wave[i] = (sound > 0) ? 1 : 0;
		phase += phase_adder;
		phase %= sample_rate_;
	}
	*/
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
