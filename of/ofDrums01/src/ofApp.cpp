#include "ofApp.h"
#include "ofxDeSynthUI.h"
#include "Sound.h"
#include "Synth.h"

DeUI::UI* UI = nullptr;
//--------------------------------------------------------------
void ofApp::setup(){
	cout << "ofDrums" << endl;

	SETTINGS.setup();

	int H = 900;
	int W = H * PANELWIDTH / PANELHEIGHT;
	ofSetWindowShape(W, H);
	ofSetFrameRate(60);

	cout << "Panel relative size " << PANELWIDTH << "x" << PANELHEIGHT << endl;

	UI = new DeUI::UI();
	UI->setup();
	UI->load_json();

	Sound::create(UI);

	screen_[0].resize(SCREEN_PX_W * SCREEN_PX_H);
	screen_[1].resize(SCREEN_PX_W * SCREEN_PX_H);

	control_thread_.start(UI, SETTINGS.control_rate);
}

//--------------------------------------------------------------
void ofApp::exit() {
	control_thread_.stop();

	UI->save_json();
	delete UI;
	UI = nullptr;
}

//--------------------------------------------------------------
void ofApp::update(){
	// Note: UI and Sound updates performed at ControlThread

	int scrw = SCREEN_PX_W;
	int scrh = SCREEN_PX_H;

	// Update screens
	for (int i = 0; i < 2; i++) {
		bool changed = false;
		for (int k = 0; k < 2; k++) {
			if (SYNTH[i * 2 + k].is_changed()) {
				SYNTH[i * 2 + k].render_to_image(screen_[i].data(), scrw, scrh,
					k * scrw / 2, 0, scrw / 2, scrh);
				changed = true;
			}
		}
		if (changed) {
			if (i == 0) UI->uiScr1->set_image_grayscale(screen_[0].data(), scrw, scrh);
			if (i == 1) UI->uiScr2->set_image_grayscale(screen_[1].data(), scrw, scrh);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	float W = ofGetWidth();
	float H = ofGetHeight();
	UI->draw(W, H);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		Sound::sound()->init_waves();
		return;
	}
	// UI keypresses are sent from ControlThread 
	// Connented: UI->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	UI->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	UI->mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	UI->mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	UI->mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	UI->mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	// cout << "Resized to " << w << " x " << h << endl;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
