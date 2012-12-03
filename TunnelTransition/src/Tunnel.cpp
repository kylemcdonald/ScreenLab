#include "Tunnel.h"

ofMatrix4x4 lerp(ofMatrix4x4& a, ofMatrix4x4& b, float t) {
	ofQuaternion trot;
	trot.slerp(t, a.getRotate(), b.getRotate());
	ofMatrix4x4 tmat(trot);
	ofVec3f apos = a.getTranslation(), bpos = b.getTranslation();
	tmat.translate(apos.interpolate(bpos, t));
	return tmat;
}

Tunnel::Tunnel()
:tunnelLength(256)
,circleResolution(21)
,segmentSubdivision(4)
,baseBlend(.8)
,dissolveBlend(.1)
,pulseBlend(.1)
,fogNear(400)
,fogFar(2500)
,tunnelSeparation(200)
,tunnelRadius(400)
,rotationChange(.01)
,rotationAmount(10)
,moveSpeed(2500)
,lerpViewRate(.05)
,segmentTiming(.5)
,randomDissolve(4.)
,useTriangles(false) {
}

void Tunnel::setup() {
	shader.setup("shader");

	cameraPath.clear();
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_LINES);
	
	ofVec3f segmentOffset(0, 0, tunnelSeparation);
	ofMatrix4x4 mat, lerpMat;
	
	for(int i = 0; i < tunnelLength; i++) {
		cameraPath.push_back(mat);
		ofQuaternion rot = mat.getRotate();
		
		for(int j = 0; j < circleResolution; j++) {			
			float theta0 = ofMap(j, 0, circleResolution, 0, TWO_PI);
			ofVec3f v0(cos(theta0), sin(theta0), 0);
			v0 *= tunnelRadius;
			mesh.addVertex(v0 * mat);
			mesh.addTexCoord(ofVec2f(i, j));
			mesh.addNormal(v0 * rot);
		}
		
		ofVec3f orientation = getOrientation(rotationChange * i);
		mat.glTranslate(0, 0, tunnelSeparation);
		mat.glRotate(orientation.x, 1, 0, 0);
		mat.glRotate(orientation.y, 0, 1, 0);
		mat.glRotate(orientation.z, 0, 0, 1);
	}
	
	for(int i = 0; i < tunnelLength - 1; i++) {
		for(int j = 0; j < circleResolution; j++) {
			mesh.addIndex(i * circleResolution + j);
			mesh.addIndex(i * circleResolution + (j + 1) % circleResolution);
			mesh.addIndex(i * circleResolution + j);
			mesh.addIndex((i + 1) * circleResolution + j);
			if(useTriangles) {
				mesh.addIndex(i * circleResolution + j);
				mesh.addIndex((i + 1) * circleResolution + (j + 1) % circleResolution);
			}
		}
	}
}

void Tunnel::syncUpdate(float clock) {
	updateTime = clock;
	float segmentPosition = updateTime * moveSpeed / tunnelSeparation;
	float remainder = fmodf(segmentPosition, 1);
	ofMatrix4x4 curView = cameraPath[((int) segmentPosition) % cameraPath.size()];
	ofMatrix4x4 nextView = cameraPath[((int) segmentPosition + 1) % cameraPath.size()];
	ofMatrix4x4 lerpView = lerp(curView, nextView, remainder);
	lerpCamera = lerp(lerpCamera, lerpView, lerpViewRate);
	if(floor(segmentPosition / segmentSubdivision + segmentTiming) !=
			floor(lastSegmentPosition / segmentSubdivision + segmentTiming)) {
		newSegment = true;
	}
	lastSegmentPosition = segmentPosition;
}

void Tunnel::draw() {
	ofNoFill();
	
	ofScale(1, 1, -1);
	
	ofPushStyle();
	ofSetLineWidth(3);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofMultMatrix(lerpCamera.getInverse());
	shader.begin();
	shader.setUniform1f("time", updateTime);
	shader.setUniform1f("baseBlend", baseBlend);
	shader.setUniform1f("dissolveBlend", dissolveBlend);
	shader.setUniform1f("pulseBlend", pulseBlend);
	shader.setUniform1f("fogNear", fogNear);
	shader.setUniform1f("fogFar", fogFar);
	shader.setUniform1f("randomDissolve", randomDissolve);
	shader.setUniform1i("segmentSubdivision", segmentSubdivision);
	mesh.draw();
	shader.end();
	ofPopStyle();
}

void Tunnel::randomize() {
	circleResolution = ofRandom(1, 30);
	tunnelSeparation = ofRandom(10, 1000);
	tunnelRadius = ofRandom(100, 1000);
	rotationChange = ofRandom(0, .1);
	rotationAmount = ofRandom(0, 25);
	moveSpeed = ofRandom(1000, 5000);
	useTriangles= ofRandomf() > .5;
}

ofVec3f Tunnel::getOrientation(float t) {
	ofVec3f rotation;
	int xr = ofNoise(t, 0, 0) * 5;
	int yr = ofNoise(0, t, 0) * 5;
	int zr = ofNoise(0, 0, t) * 5;
	switch(xr) {
		case 0: rotation.x = -rotationAmount; break;
		case 2: rotation.x = +rotationAmount; break;
	}
	switch(yr) {
		case 0: rotation.y = -rotationAmount; break;
		case 2: rotation.y = +rotationAmount; break;
	}
	switch(zr) {
		case 0: rotation.z = -rotationAmount; break;
		case 2: rotation.z = +rotationAmount; break;
	}
	return rotation;
}

bool Tunnel::isSegmentNew() {
	bool curNewSegment = newSegment;
	newSegment = false;
	return curNewSegment;
}