#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	model.loadModel("octave-mounts.dae");
	faceShader.setup("shader");
	//wireframeShader.setup("wireframeShader");
	glEnable(GL_DEPTH_TEST);
}

void ofApp::update() {
	
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	ofRotateX(180);
	
	faceShader.begin();
	faceShader.setUniform1f("elapsedTime", ofGetElapsedTimef());
	model.drawWireframe();
	faceShader.end();
	
	//wireframeShader.begin();
	//model.drawWireframe();
	//wireframeShader.end();
	
	cam.end();
}