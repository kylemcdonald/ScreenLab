#include "ofAppGlutWindow.h"
#include "ofMain.h"
#include "ofxAutoControlPanel.h"

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

class ofApp : public ofBaseApp {
public:
	ofEasyCam cam;
	ofxAutoControlPanel gui;
	
	float _(string name) {
		return gui.getValueF(name);
	}
	
	void setup() {
		ofSetVerticalSync(true);
		glEnable(GL_DEPTH_TEST);
		
		gui.setup(250, 700);
		gui.addPanel("Settings");
		gui.addSlider("fogNear", 400, 0, 1000);
		gui.addSlider("fogFar", 8000, 0, 10000);
		gui.addSlider("tunnelLength", 64, 0, 1000, true);
		gui.addSlider("tunnelSeparation", 200, 0, 1000);
		gui.addSlider("tunnelRadius", 400, 0, 1000);
		gui.addSlider("rotationChange", .05, 0, 1);
		gui.addSlider("xRotation", 10, 0, 45);
		gui.addSlider("yRotation", 10, 0, 45);
		gui.addSlider("moveSpeed", 2500, 0, 5000);
		gui.addSlider("circleResolution", 21, 1, 30, true);
	}
	
	void update() {
	}
	
	ofVec2f getOrientation(float t) {
		float xRotation = _("xRotation");
		float yRotation = _("yRotation");
		int choice = ofNoise(t, 0) * 5;
		switch(choice) {
			case 0: return ofVec2f(-xRotation, 0);
			case 1: return ofVec2f(+xRotation, 0);
			case 2: return ofVec2f(0, -yRotation);
			case 3: return ofVec2f(0, +yRotation);
		}
		return ofVec2f();
	}
	
	void draw() {
		ofBackground(0);
		ofNoFill();
		
		cam.begin();
		ofScale(1, 1, -1);
		enableFog(_("fogNear"), _("fogFar"));
		int tunnelLength = _("tunnelLength");
		float tunnelRadius = _("tunnelRadius");
		float tunnelSeparation = _("tunnelSeparation");
		float rotationChange = _("rotationChange");
		float moveSpeed = _("moveSpeed");
		float moveOffset = moveSpeed * ofGetElapsedTimef() / tunnelSeparation;
		int moveWhole = floor(moveOffset);
		float moveFraction = fmodf(moveOffset, 1);
		
		ofVboMesh tunnelSegment;
		tunnelSegment.setMode(OF_PRIMITIVE_LINES);
		ofVec3f segmentOffset(0, 0, tunnelSeparation);
		int circleResolution = _("circleResolution");
		for(int i = 0; i < circleResolution; i++) {
			float theta0 = ofMap(i + 0, 0, circleResolution - 1, 0, 360);
			float theta1 = ofMap(i + 1, 0, circleResolution - 1, 0, 360);
			ofVec2f v0(tunnelRadius, 0), v1(tunnelRadius, 0);
			v0.rotate(theta0);
			v1.rotate(theta1);
			tunnelSegment.addVertex(v0);
			tunnelSegment.addVertex(v1);
			tunnelSegment.addVertex(v0);
			tunnelSegment.addVertex(segmentOffset + v0);
			tunnelSegment.addVertex(segmentOffset + v0);
			tunnelSegment.addVertex(segmentOffset + v1);
		}
		
		ofPushMatrix();
		
		ofVec2f lerpOrientation = ofVec2f().interpolate(getOrientation(rotationChange * (1 + moveWhole)), moveFraction);
		ofRotateX(-lerpOrientation.x);
		ofRotateY(-lerpOrientation.y);
		ofTranslate(0, 0, -moveFraction * tunnelSeparation);
		
		for(int i = 0; i < tunnelLength; i++) {
			if(i > 0) {
				ofVec2f orientation = getOrientation(rotationChange * (i + moveWhole));
				ofRotateX(orientation.x);
				ofRotateY(orientation.y);
			}
			ofSetColor(255);
			tunnelSegment.draw();
			ofTranslate(0, 0, tunnelSeparation);
		}
		ofPopMatrix();
		disableFog();
		cam.end();
	}
};

int main() {
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth");
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
