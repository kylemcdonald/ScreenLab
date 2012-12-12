#include "ofApp.h"

int tetrahedronVertexCount = 4;
float tetrahedronVertices[] = {
-1, -1, -1, 
1, 1, -1, 
1, -1, 1, 
-1, 1, 1};
int tetrahedronIndexCount = 12;
int tetrahedronIndices[] = {
1, 2, 3, 
1, 0, 2, 
3, 2, 0, 
0, 1, 3};

int octahedronVertexCount = 6;
float octahedronVertices[] = {
1, 0, 0, 
0, -1, 0, 
-1, 0, 0, 
0, 1, 0, 
0, 0, 1, 
0, 0, -1};
int octahedronIndexCount = 24;
int octahedronIndices[] = {
4, 0, 1, 
4, 1, 2, 
4, 2, 3, 
4, 3, 0, 
5, 1, 0, 
5, 2, 1, 
5, 3, 2, 
5, 0, 3}

int cubeVertexCount = 8;
float cubeVertices[] = {
-1, -1, -1, 
1, -1, -1, 
1, 1, -1, 
-1, 1, -1, 
-1, -1, 1, 
1, -1, 1, 
1, 1, 1, 
-1, 1, 1};
int cubeIndexCount = 36;
int cubeIndices[] = {
0, 1, 2,  0, 2, 3,   
5, 4, 7,  5, 7, 6,   
6, 2, 1,  6, 1, 5,   
3, 7, 4,  3, 4, 0,   
7, 3, 2,  7, 2, 6,   
5, 1, 0,  5, 0, 4};

int icosahedronVertexCount = 12;
float icosahedronVertices[] = {
0, -0.525731, 0.850651, 
0.850651, 0, 0.525731, 
0.850651, 0, -0.525731, 
-0.850651, 0, -0.525731, 
-0.850651, 0, 0.525731, 
-0.525731, 0.850651, 0, 
0.525731, 0.850651, 0, 
0.525731, -0.850651, 0, 
-0.525731, -0.850651, 0, 
0, -0.525731, -0.850651, 
0, 0.525731, -0.850651, 
0, 0.525731, 0.850651};
int icosahedronIndexCount = 60;
int icosahedronIndices[] = {
6, 2, 1, 
2, 7, 1, 
5, 4, 3, 
8, 3, 4, 
11, 5, 6, 
10, 6, 5, 
2, 10, 9, 
3, 9, 10, 
9, 8, 7, 
0, 7, 8, 
1, 0, 11, 
4, 11, 0, 
10, 2, 6, 
11, 6, 1, 
10, 5, 3, 
11, 4, 5, 
9, 7, 2, 
0, 1, 7, 
8, 9, 3, 
0, 8, 4};

int dodecahedronVertexCount = 20;
float dodecahedronVertices[] = {
-0.57735, -0.57735, 0.57735, 
0.934172, 0.356822, 0, 
0.934172, -0.356822, 0, 
-0.934172, 0.356822, 0, 
-0.934172, -0.356822, 0, 
0, 0.934172, 0.356822, 
0, 0.934172, -0.356822, 
0.356822, 0, -0.934172, 
-0.356822, 0, -0.934172, 
0, -0.934172, -0.356822, 
0, -0.934172, 0.356822, 
0.356822, 0, 0.934172, 
-0.356822, 0, 0.934172, 
0.57735, 0.57735, -0.57735, 
0.57735, 0.57735, 0.57735, 
-0.57735, 0.57735, -0.57735, 
-0.57735, 0.57735, 0.57735, 
0.57735, -0.57735, -0.57735, 
0.57735, -0.57735, 0.57735, 
-0.57735, -0.57735, -0.57735};
int dodecahedronIndexCount = 108;
int dodecahedronIndices[] = {
1, 2, 18,  1, 18, 11,  1, 11, 14, 
1, 13, 7,  1, 7, 17,  1, 17, 2, 
3, 4, 19,  3, 19, 8,  3, 8, 15, 
3, 16, 12,  3, 12, 0,  3, 0, 4, 
3, 15, 6,  3, 6, 5,  3, 5, 16, 
1, 14, 5,  1, 5, 6,  1, 6, 13, 
2, 17, 9,  2, 9, 10,  2, 10, 18, 
4, 0, 10,  4, 10, 9,  4, 9, 19, 
7, 8, 19,  7, 19, 9,  7, 9, 17, 
6, 15, 8,  6, 8, 7,  6, 7, 13, 
5, 14, 11,  5, 11, 12,  5, 12, 16, 
10, 0, 12,  10, 12, 11,  10, 11, 18};
	
void addVertices(ofMesh& mesh, float[] vertices, int n) {
	for(int i = 0; i < n; i++) {
		mesh.addIndex(indices[i]);
	}
}

void addIndices(ofMesh& mesh, int[] indices, int n) {
	for(int i = 0; i < n; i++) {
		mesh.addIndex(indices[i]);
	}
}

ofMesh buildTetrahedron() {
	ofMesh mesh;
	addVertices(mesh, tetrahedronVertices, tetrahedronVertexCount);
	addIndices(mesh, tetrahedronIndices, tetrahedronIndexCount);
	return mesh;
}

void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	vbo = buildTetrahedron();
	//loadMesh(0);
	
	vector<ofMatrix4x4> transforms;
	for(int i = 0; i < 16; i++) {
		ofMatrix4x4 transform;
		float scale = ofRandom(10, 100);
		transform.glTranslate(ofVec3f(ofRandomf(), ofRandomf(), ofRandomf()) * ofGetWidth());
		transform.glRotate(ofQuaternion(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1));
		transform.glScale(scale, scale, scale);
		transforms.push_back(transform);
	}
	setTransforms(transforms);
}

void ofApp::update() {
	
}

void ofApp::draw() {
	ofBackground(0);
	ofSetColor(255);
	cam.begin();
	for(int i = 0; i < transforms.size(); i++) {
		ofPushMatrix();
		ofMultMatrix(transforms[i]);
		vbo.drawWireframe();
		ofPopMatrix();
	}
	cam.end();
}

void ofApp::loadMesh(int index) {
	vbo.load(ofToString(index) + ".ply");
	for(int i = 0; i < vbo.getNumVertices(); i++) {
		cout << vbo.getVertex(i) << endl;
	}
}

void ofApp::setTransforms(const vector<ofMatrix4x4>& transforms) {
	this->transforms = transforms;
}