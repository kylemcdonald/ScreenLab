#pragma once

#include "ofMain.h"
#include "ofAutoShader.h"

class Tunnel {
protected:
	ofVboMesh mesh;
	vector<ofMatrix4x4> cameraPath;
	ofMatrix4x4 lerpCamera;
	bool newSegment;
	float updateTime, lastSegmentPosition;
	
	ofAutoShader shader;
	
	ofVec3f getOrientation(float t);

public:
	int
	tunnelLength,
	circleResolution,
	segmentSubdivision;
	float 
	baseBlend,
	dissolveBlend,
	pulseBlend,
	fogNear,
	fogFar,
	tunnelSeparation,
	tunnelRadius,
	rotationChange,
	rotationAmount,
	moveSpeed,
	lerpViewRate,
	segmentTiming,
	randomDissolve;
	bool
	useTriangles;
	
	Tunnel();
	
	void setup();
	void update();
	void draw();
	void randomize();
	
	bool isSegmentNew();
};