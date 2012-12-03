#pragma once

#include "ofMain.h"

#include "ofxAssimpModelLoader.h"
#include "ofAutoShader.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam cam;
	ofxAssimpModelLoader model;
	ofAutoShader faceShader;//, wireframeShader;
};