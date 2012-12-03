#pragma once

#include "ofMain.h"
#include "SyncedUpdatable.h"

#ifdef AUTO_SHADER
#include "ofAutoShader.h"
#endif

class Tunnel : public SyncedUpdatable {
protected:
	ofVboMesh mesh;
	vector<ofMatrix4x4> cameraPath;
	ofMatrix4x4 lerpCamera;
	bool newSegment;
	float updateTime, lastSegmentPosition;
	
#ifdef AUTO_SHADER
	ofAutoShader shader;
#else
	ofShader shader;
#endif
	
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
	
	bool isSegmentNew();
};