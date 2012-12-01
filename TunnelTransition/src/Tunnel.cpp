#include "Tunnel.h"

void enableFog(float near, float far) {
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4]= {0, 0, 0, 1};
	glFogfv(GL_FOG_COLOR, fogColor);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glFogf(GL_FOG_START, near);
	glFogf(GL_FOG_END, far);
}

void disableFog() {
	glDisable(GL_FOG);
}

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
,fogNear(400)
,fogFar(2500)
,tunnelSeparation(200)
,tunnelRadius(400)
,rotationChange(.01)
,rotationAmount(10)
,moveSpeed(2500)
,lerpViewRate(.05)
,useTriangles(false) {
}

void Tunnel::setup() {
	cameraPath.clear();
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_LINES);
	
	ofVec3f segmentOffset(0, 0, tunnelSeparation);
	ofMatrix4x4 mat, lerpMat;
	for(int i = 0; i < tunnelLength; i++) {
		cameraPath.push_back(mat);
		
		for(int j = 0; j < circleResolution; j++) {			
			float theta0 = ofMap(j + 0, 0, circleResolution, 0, TWO_PI);
			float theta1 = ofMap(j + 1, 0, circleResolution, 0, TWO_PI);
			ofVec3f v0(cos(theta0), sin(theta0), 0);
			ofVec3f v1(cos(theta1), sin(theta1), 0);
			v0 *= tunnelRadius;
			v1 *= tunnelRadius;
			mesh.addVertex((v0) * mat);
			mesh.addVertex((v1) * mat);
			if(useTriangles) {
				mesh.addVertex((v0) * mat);
				mesh.addVertex((segmentOffset + v1) * mat);
			} else {
				mesh.addVertex((v0) * mat);
				mesh.addVertex((segmentOffset + v0) * mat);
				mesh.addVertex((segmentOffset + v0) * mat);
				mesh.addVertex((segmentOffset + v1) * mat);
			}
		}
		
		ofVec3f orientation = getOrientation(rotationChange * i);
		mat.glTranslate(0, 0, tunnelSeparation);
		mat.glRotate(orientation.x, 1, 0, 0);
		mat.glRotate(orientation.y, 0, 1, 0);
		mat.glRotate(orientation.z, 0, 0, 1);
	}
}

void Tunnel::update() {
	float t = ofGetElapsedTimef() * moveSpeed / tunnelSeparation;
	float remainder = fmodf(t, 1);
	ofMatrix4x4 curView = cameraPath[((int) t) % cameraPath.size()];
	ofMatrix4x4 nextView = cameraPath[((int) t + 1) % cameraPath.size()];
	ofMatrix4x4 lerpView = lerp(curView, nextView, remainder);
	
	lerpCamera = lerp(lerpCamera, lerpView, lerpViewRate);
}

void Tunnel::draw() {
	ofNoFill();
	
	enableFog(fogNear, fogFar);
	ofScale(1, 1, -1);
	
	ofPushStyle();
	ofSetLineWidth(3);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255, 190);
	ofMultMatrix(lerpCamera.getInverse());
	mesh.draw();
	ofPopStyle();
	
	disableFog();
}

void Tunnel::randomize() {
	circleResolution = ofRandom(1, 30);
	tunnelSeparation = ofRandom(10, 1000);
	tunnelRadius = ofRandom(100, 1000);
	rotationChange = ofRandom(0, .1);
	rotationAmount = ofRandom(0, 45);
	moveSpeed = ofRandom(0, 5000);
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