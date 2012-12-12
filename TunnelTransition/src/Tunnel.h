#pragma once

#include "ofMain.h"
#include "SyncedUpdatable.h"

#include "ofAutoShader.h"

class Tunnel : public SyncedUpdatable {
protected:
	ofVboMesh mesh;
	vector<ofMatrix4x4> cameraPath;
	ofMatrix4x4 lerpCamera;
	bool newSegment;
	float updateTime, lastSegmentPosition;
	
	ofAutoShader shader;
	
	ofVec3f getOrientation(float t);
	
	void syncUpdate(float clock);

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
	void draw();
	void randomize();
	void save(string filename);
	
	bool isSegmentNew();
};