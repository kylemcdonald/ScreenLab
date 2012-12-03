#include "ofAppGlutWindow.h"
#include "ofMain.h"

ofVec3f getNormal(const ofVec3f& v1, const ofVec3f& v2, const ofVec3f& v3) {
	ofVec3f a = v1 - v2;
	ofVec3f b = v3 - v2;
	ofVec3f normal = b.cross(a);
	normal.normalize();
	return normal;
}

void buildNormals(ofMesh& mesh) {
	vector<ofVec3f>& vertices = mesh.getVertices();
	for(int i = 0; i < mesh.getNumVertices(); i += 3) {
		ofVec3f normal = getNormal(
															 mesh.getVertices()[i+0],
															 mesh.getVertices()[i+1],
															 mesh.getVertices()[i+2]);
		for(int j = 0; j < 3; j++) {
			mesh.addNormal(normal);
		}
	}
}

ofMesh convertFromIndices(const ofMesh& mesh) {
	ofMesh result;
	// have to do a const_cast because ofMesh::get*() is not const correct
	ofMesh& cmesh = const_cast<ofMesh&>(mesh);
	int vertices = mesh.getNumVertices();
	int colors = mesh.getNumColors();
	int normals = mesh.getNumNormals();
	int texcoords = mesh.getNumTexCoords();
	int indices = mesh.getNumIndices();
	for(int i = 0; i < indices; i++) {
		int cur = cmesh.getIndex(i);
		if(vertices > 0) {
			result.addVertex(cmesh.getVertex(cur));
		}
		if(colors > 0) {
			result.addColor(cmesh.getColor(cur));
		}
		if(normals > 0) {
			result.addNormal(cmesh.getNormal(cur));
		}
		if(texcoords > 0) {
			result.addTexCoord(cmesh.getTexCoord(cur));
		}
	}
	return result;
}

class ofApp : public ofBaseApp {
public:
	ofImage depthmap, texture;
	ofVboMesh landscape, landscapeWhite;
	ofEasyCam cam;
	int w, h;
	vector<ofLight> lights;
	
	void setup() {
		lights.resize(6);
		for(int i = 0; i < lights.size(); i++) {
			lights[i].setAttenuation(1., .1, 0);
			lights[i].setAmbientColor(ofColor::fromHsb(ofRandom(0, 255), 128, 255));
		}
		
		depthmap.loadImage("depthmap.png");
		texture.loadImage("texture.png");
		
		ofMesh mesh;
		w = depthmap.getWidth(), h = depthmap.getHeight();
		for(int y = 0; y < h; y++) {
			for(int x = 0; x < w; x++) {
				mesh.addVertex(ofVec3f(x, y, depthmap.getColor(x, y).getBrightness() / 2.));
				mesh.addColor(texture.getColor(x, y));
			}
		}
		for(int y = 0; y < h - 1; y++) {
			for(int x = 0; x < w - 1; x++) {
				int nw = y * w + x;
				int ne = nw + 1;
				int sw = nw + w;
				int se = sw + 1;
				mesh.addIndex(nw);
				mesh.addIndex(ne);
				mesh.addIndex(sw);
				mesh.addIndex(ne);
				mesh.addIndex(se);
				mesh.addIndex(sw);
			}
		}
		mesh = convertFromIndices(mesh);
		buildNormals(mesh);
		landscape = mesh;
		landscape.clearNormals();
		landscapeWhite = mesh;
		landscapeWhite.clearColors();
    glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
	
	void update() {
		for(int i = 0; i < lights.size(); i++) {
			lights[i].setPosition(ofNoise(ofGetElapsedTimef(), 0, i) * w, ofNoise(0, ofGetElapsedTimef(), i) * h, +14);
		}
	}
	
	void draw() {
		ofBackground(0);
		cam.begin();
		ofTranslate(-w / 2, -h / 2);
		
		ofEnableLighting();
		for(int i = 0; i < lights.size(); i++) {
			lights[i].enable();
		}
		ofSetColor(mouseX);
		landscapeWhite.draw();
		for(int i = 0; i < lights.size(); i++) {
			lights[i].disable();
		}
		ofDisableLighting();
		
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetColor(255);
		ofTranslate(0, 0, 1);
		landscape.draw();
		cam.end();
	}
};

int main() {
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgb double depth alpha samples>=4");
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
