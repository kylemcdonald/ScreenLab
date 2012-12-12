#pragma once

#define AUTO_SHADER

#include "ofxAutoControlPanel.h"
#include "Tunnel.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp {
public:
	ofEasyCam cam;
	ofxAutoControlPanel gui;
	Tunnel tunnel;
	ofxMidiOut midi;
	
	float _(string name);
	
	void setup();	
	void update();
	void draw();
};
