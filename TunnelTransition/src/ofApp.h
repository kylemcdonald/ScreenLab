#pragma once

#include "ofxAutoControlPanel.h"
#include "Tunnel.h"

class ofApp : public ofBaseApp {
public:
	ofEasyCam cam;
	ofxAutoControlPanel gui;
	Tunnel tunnel;
	
	float _(string name);
	
	void setup();	
	void update();
	void draw();
};
