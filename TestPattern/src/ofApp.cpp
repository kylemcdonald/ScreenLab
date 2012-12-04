#include "ofApp.h"

float octaveHeight = 193.2;
float octaveOuterRadius = 340.1;
float octaveInnerRadius = 314.211;

void ofApp::setup() {
	int rings = 3, resolution = 8;
	float length = octaveHeight, radius = 340.1;
	
	for(int i = 0; i < rings; i++) {
		ofVec3f offset(0, 0, ofMap(i, 0, rings - 1, 0, length));
		for(int j = 0; j < resolution; j++) {
			float theta = ofMap(j, 0, resolution, 0, 360);
			ofVec2f cur(radius, 0);
			cur.rotate(theta);
			mesh.addVertex(offset + cur);
		}
	}
	
	for(int i = 0; i < rings - 1; i++) {
		for(int j = 0; j < resolution; j++) {
			int sw = i * resolution + j, se = sw + 1;
			if(j + 1 == resolution) {
				se -= resolution;
			}
			int nw = sw + resolution, ne = se + resolution;
			mesh.addTriangle(sw, se, nw);
			mesh.addTriangle(nw, se, ne);
		}
	}
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

void ofApp::update() {
	
}

void ofApp::draw() {
	cam.begin();
	//ofDrawGrid(1000, 10, true, false, false, true);
	for(int i = 0; i < 8; i++) {
		ofPushMatrix();
		ofRotateZ(i * 45);
		ofTranslate(octaveInnerRadius, 0, 0);
		ofPushMatrix();
		ofTranslate(0, 0, octaveHeight / 2);
		ofDrawGrid(octaveHeight / 2, 2, false, true, false, false);
		ofPopMatrix();
		ofPushMatrix();
		ofTranslate(-octaveHeight / 2, 0, 0);
		ofDrawGrid(octaveHeight / 2, 2, false, false, false, true);
		ofPopMatrix();
		ofPopMatrix();
	}
	ofPushMatrix();
	ofRotateZ(45. / 2);
	mesh.drawWireframe();
	ofPopMatrix();
	cam.end();
}