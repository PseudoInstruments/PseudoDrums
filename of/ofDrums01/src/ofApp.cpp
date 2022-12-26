#include "ofApp.h"
#include "ofxDeSynthUI.h"
#include "Sound.h"
#include "Synth.h"

DeUI::UI* UI = nullptr;
//--------------------------------------------------------------
void ofApp::setup(){
	int W = 1080;
	int H = W * 2 / 3;
	ofSetWindowShape(1080, H);
	ofSetFrameRate(60);

	UI = new DeUI::UI();
	UI->setup();
	UI->load_json();

	Sound::create(UI);
}

//--------------------------------------------------------------
void ofApp::exit() {
	UI->save_json();
	delete UI;
	UI = nullptr;
}

//--------------------------------------------------------------
void ofApp::update(){
	UI->update();
	//UI->LED1 = UI->PAD1;


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
	UI->keyPressed(key);
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
