#pragma once

#include "ofMain.h"

class Tunnel {
protected:
	ofVboMesh mesh;
	vector<ofMatrix4x4> cameraPath;
	ofMatrix4x4 lerpCamera;

public:
	int
	tunnelLength,
	circleResolution;
	float 
	fogNear,
	fogFar,
	tunnelSeparation,
	tunnelRadius,
	rotationChange,
	rotationAmount,
	moveSpeed,
	lerpViewRate;
	
	Tunnel();
	
	void setup();
	void draw();
	ofVec3f getOrientation(float t);
};