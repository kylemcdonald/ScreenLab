#include "ofAppGlutWindow.h"
#include "ofMain.h"

class Meteorite {
	
};

class Particle {
protected:
	ofVboMesh* mesh;
	
	ofVec3f rotationAxis;
	float rotation, rotationSpeed;
	
	ofVec3f gravity, divergence;
	ofVec3f position, velocity;
	float size;
	
	int iteration;
	
public:
	Particle()
	:iteration(0) {
	}
	
	void setup(ofVboMesh& mesh) {
		this->mesh = &mesh;
		rotationAxis.set(ofRandomf(), ofRandomf(), ofRandomf());
		position.set(200, 0, 0);
		velocity.set(ofRandomf(), ofRandomf(), ofRandomf());
		position.rotate(ofRandom(360), ofRandom(360), ofRandom(360));
		velocity *= 8;
		size = ofRandom(10, 50);
		rotationSpeed = ofRandom(10, 400);
		gravity.set(0, -.5, 0);
		divergence.set(ofRandomf(), 0, ofRandomf());
		divergence *= .001;
	}
	
	void update(float t) {
		rotation = rotationSpeed * t;
		int targetIteration = 60 * t;
		while(iteration < targetIteration) {
			iterativeUpdate();
			iteration++;
		}
	}
	
	void iterativeUpdate() {
		float distance = position.length();
		if(distance > 1600) {
			setup(*mesh);
		} else {
			velocity += gravity;
			velocity += divergence * distance;
			position += velocity;
			velocity *= .99;
		}
	}
	
	void draw() {
		ofPushMatrix();
		ofTranslate(position);
		ofRotate(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
		ofScale(size, size, size);
		ofRotateX(-43);
		mesh->draw();
		ofPopMatrix();
	}	
};

class ofApp : public ofBaseApp {
public:
	ofVboMesh tetrahedron;
	ofEasyCam cam;
	vector<Particle> particles;
	
	static ofMesh buildTetrahedron(ofPrimitiveMode mode) {
		ofMesh mesh;
		mesh.setMode(mode);
		mesh.addVertex(ofVec3f(+1, 0, -1 / sqrt(2)));
		mesh.addVertex(ofVec3f(-1, 0, -1 / sqrt(2)));
		mesh.addVertex(ofVec3f(0, +1, +1 / sqrt(2)));
		mesh.addVertex(ofVec3f(0, -1, +1 / sqrt(2)));
		if(mode == OF_PRIMITIVE_LINES) {
			mesh.addIndex(0);
			mesh.addIndex(1);
			mesh.addIndex(1);
			mesh.addIndex(2);
			mesh.addIndex(2);
			mesh.addIndex(0);
			mesh.addIndex(0);
			mesh.addIndex(3);
			mesh.addIndex(1);
			mesh.addIndex(3);
			mesh.addIndex(2);
			mesh.addIndex(3);
		}
		return mesh;
	}
	
	void setup() {
		ofSetVerticalSync(true);
		tetrahedron = buildTetrahedron(OF_PRIMITIVE_LINES);
		particles.resize(1024);
		for(int i = 0; i < particles.size(); i++) {
			particles[i].setup(tetrahedron);
		}
	}
	
	void update() {
		float t = ofGetElapsedTimef();
		for(int i = 0; i < particles.size(); i++) {
			particles[i].update(t);
		}
	}
	
	void setupBlending() {
		ofColor tetrahedronRed(231, 49, 35);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetLineWidth(3);
		ofSetColor(tetrahedronRed, 190);
	}
	
	void draw() {
		ofBackground(0);
		cam.begin();
		ofRotateY(ofGetElapsedTimef() * 30);
		setupBlending();
		
		ofScale(.2,.2,.2);
		
		ofPushMatrix();
		ofTranslate(0, 200, 0);
		ofScale(200, 200, 200);
		ofRotateX(-43);
		ofSetLineWidth(8);
		tetrahedron.draw();
		ofPopMatrix();
		
		ofSetLineWidth(3);
		for(int i = 0; i < particles.size(); i++) {
			particles[i].draw();
		}
		cam.end();
	}
};

int main() {
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth");
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
