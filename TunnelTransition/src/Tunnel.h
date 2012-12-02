#pragma once

#include "ofMain.h"

class Tunnel {
protected:
	ofVboMesh mesh;
	vector<ofMatrix4x4> cameraPath;
	ofMatrix4x4 lerpCamera;
	bool newSegment;
	float lastt;
	
	ofVec3f getOrientation(float t);

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
	lerpViewRate,
	segmentTiming;
	bool
	useTriangles;
	
	Tunnel();
	
	void setup();
	void update();
	void draw();
	void randomize();
	
	bool isSegmentNew();
};