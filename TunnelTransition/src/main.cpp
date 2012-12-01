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
	ofVboMesh mesh;
	vector<ofMatrix4x4> cameraPath;
	ofMatrix4x4 lerpCamera;
	
	float _(string name) {
		return gui.getValueF(name);
	}
	
	void setup() {
		ofSetVerticalSync(true);
		
		gui.setup(250, 700);
		gui.addPanel("Settings");
		gui.addSlider("fogNear", 400, 0, 1000);
		gui.addSlider("fogFar", 2500, 0, 4000);
		gui.addSlider("tunnelLength", 256, 0, 512, true);
		gui.addSlider("tunnelSeparation", 200, 0, 1000);
		gui.addSlider("tunnelRadius", 400, 0, 1000);
		gui.addSlider("rotationChange", .01, 0, .1);
		gui.addSlider("rotationAmount", 10, 0, 45);
		gui.addSlider("circleResolution", 21, 1, 30, true);
		gui.addSlider("moveSpeed", 2500, 0, 5000);
		gui.addSlider("lerpViewRate", .05, 0, .1);
		
		buildMesh();
	}
	
	void buildMesh() {
		cameraPath.clear();
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_LINES);
		
		int tunnelLength = _("tunnelLength");
		float tunnelRadius = _("tunnelRadius");
		int circleResolution = _("circleResolution");
		float tunnelSeparation = _("tunnelSeparation");
		float rotationChange = _("rotationChange");
		
		ofVec3f segmentOffset(0, 0, tunnelSeparation);
		ofMatrix4x4 mat, lerpMat;
		for(int i = 0; i < tunnelLength; i++) {
			cameraPath.push_back(mat);
			
			for(int j = 0; j < circleResolution; j++) {			
				float theta0 = ofMap(j + 0, 0, circleResolution - 1, 0, TWO_PI);
				float theta1 = ofMap(j + 1, 0, circleResolution - 1, 0, TWO_PI);
				ofVec3f v0(cos(theta0), sin(theta0), 0);
				ofVec3f v1(cos(theta1), sin(theta1), 0);
				v0 *= tunnelRadius;
				v1 *= tunnelRadius;
				mesh.addVertex((v0) * mat);
				mesh.addVertex((v1) * mat);
				mesh.addVertex((v0) * mat);
				mesh.addVertex((segmentOffset + v0) * mat);
				mesh.addVertex((segmentOffset + v0) * mat);
				mesh.addVertex((segmentOffset + v1) * mat);
			}
			
			ofVec3f orientation = getOrientation(rotationChange * i);
			mat.glTranslate(0, 0, tunnelSeparation);
			mat.glRotate(orientation.x, 1, 0, 0);
			mat.glRotate(orientation.y, 0, 1, 0);
			mat.glRotate(orientation.z, 0, 0, 1);
		}
	}
	
	void update() {
		if(gui.hasValueChangedInPanel("Settings")) {
			buildMesh();
			gui.clearAllChanged();
		}
	}
	
	ofVec3f getOrientation(float t) {
		ofVec3f rotation;
		float rotationAmount = _("rotationAmount");
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
	
	ofMatrix4x4 lerp(ofMatrix4x4& a, ofMatrix4x4& b, float t) {
		ofQuaternion trot;
		trot.slerp(t, a.getRotate(), b.getRotate());
		ofMatrix4x4 tmat(trot);
		ofVec3f apos = a.getTranslation(), bpos = b.getTranslation();
		tmat.translate(apos.interpolate(bpos, t));
		return tmat;
	} 
	
	void draw() {
		ofBackground(0);
		ofNoFill();
		
		float t = ofGetElapsedTimef() * _("moveSpeed") / _("tunnelSeparation");
		float remainder = fmodf(t, 1);
		ofMatrix4x4 curView = cameraPath[((int) t) % cameraPath.size()];
		ofMatrix4x4 nextView = cameraPath[((int) t + 1) % cameraPath.size()];
		ofMatrix4x4 lerpView = lerp(curView, nextView, remainder);
		
		lerpCamera = lerp(lerpCamera, lerpView, _("lerpViewRate"));
		
		cam.begin();
		enableFog(_("fogNear"), _("fogFar"));
		ofScale(1, 1, -1);
		
		ofPushStyle();
		ofSetLineWidth(3);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetColor(255, 190);
		ofMultMatrix(lerpCamera.getInverse());
		mesh.draw();
		ofPopStyle();
		
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
