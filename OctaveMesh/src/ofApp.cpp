#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	model.loadModel("octave-mounts.dae");
}

void ofApp::update() {
	
}

void ofApp::draw() {
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	ofBackground(0);
	glEnable(GL_DEPTH_TEST);
	ofSetColor(255);
	ofScale(1, -1, 1);
	ofRotateX(180);
	ofRotateX(90);
	model.drawWireframe();
}