#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void loadMesh(int index);
	void setTransforms(const vector<ofMatrix4x4>& transforms);
	
	ofVboMesh vbo;
	vector<ofMatrix4x4> transforms;
	ofEasyCam cam;
};