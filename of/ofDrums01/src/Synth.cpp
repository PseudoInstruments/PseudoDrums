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
void Synth::init_wave() {
	int duration_ms = util::mapi_clamp(*Duration, pot_min, pot_max, 
		SETTINGS.duration_ms0, SETTINGS.duration_ms1);

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
	
	//Debug print
	
	cout << "Drum " << id_+1 << " update" << endl;
	cout << "duration_ms " << duration_ms << endl;
	cout << "wave_n " << wave_n_ << endl;
	cout << "Freq " << util::note_to_hz_int(note0) 
		<< " -- " << util::note_to_hz_int(note1) << endl;
	cout << "Noise " << timbre_noise << endl;;

	for (int i=0; i<wave_n_; i++) {
	  if (i % 80 == 0 && i > 0) cout << endl;
	  cout << int(wavebuf_[i]);
	}
	cout << endl;

	//Serial.println("MIDI to freq:");
	//for (int i=0; i<127; i++) {
	//  Serial.print(i); Serial.print(" ");Serial.println(m_to_f_int(i)); 
	//}
}

//--------------------------------------------------------------
void Synth::play_wave()
{

}

//--------------------------------------------------------------
void Synth::audio_add_stereo(float* data, int nframes) {

}

//--------------------------------------------------------------
