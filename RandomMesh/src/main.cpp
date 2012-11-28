#include "ofAppGlutWindow.h"
#include "ofMain.h"

#include "ofxDelaunay.h"
#include "ofMeshUtils.h"

class ofApp : public ofBaseApp {
public:
	typedef float MapType;
	
	static const float topOffset = 10;
	static const int scaleFactor = 4;
	static const int range = 100;
	static const float density = 64;
	
	int width, height, n;
	vector<MapType> ping, pong;
	bool side;
	ofImage out;
	
	ofLight light;
	ofEasyCam cam;
	ofxDelaunay delaunay;
	ofMesh triangles, subtriangles;
	vector<int> tops;
	
	MapType& get(vector<MapType>& data, int x, int y) {
		return data.at(y * width + x);
	}
	
	void setup() {
		ofSetVerticalSync(true);
		width = ofGetWidth() / scaleFactor, height = ofGetHeight() / scaleFactor;
		n = width * height;
		side = false;
		ping.resize(n, 0);
		pong.resize(n, 0);
		out.allocate(width, height, OF_IMAGE_GRAYSCALE);
		
		glShadeModel(GL_SMOOTH);
		
		light.enable();
		
		vector<ofVec2f> points;
		float minDistance = 10;
		for(int i = 0; i < 640; i++) {
			bool good = false;
			while(!good) {
				ofVec2f cur(ofRandomWidth(), ofRandomHeight());
				good = true;
				for(int j = 0; j < i; j++) {
					if(cur.distance(points[j]) < minDistance) {
						good = false;
						break;
					}
				}
				if(good) {
					points.push_back(cur);
				}
			}
		}
		for(int i = 0; i < points.size(); i++) {
			delaunay.addPoint(points[i]);
		}
		delaunay.triangulate();
		triangles = delaunay.triangleMesh;
		
		// triangles are inverted
		for(int i = 0; i < triangles.getNumIndices(); i += 3) {
			swap(triangles.getIndices()[i + 0], triangles.getIndices()[i + 2]);
		}
		
		subtriangles = triangles;
		subtriangles.clearIndices();
		
		ofColor base = ofColor(200);
		ofColor top = ofColor(255);
		for(int i = 0; i < subtriangles.getNumVertices(); i++) {
			subtriangles.addColor(base);
		}
		
		for(int i = 0; i < triangles.getNumIndices(); i += 3) {
			int i0 = triangles.getIndex(i + 0);
			int i1 = triangles.getIndex(i + 1);
			int i2 = triangles.getIndex(i + 2);
			ofVec2f t0 = triangles.getVertex(i0);
			ofVec2f t1 = triangles.getVertex(i1);
			ofVec2f t2 = triangles.getVertex(i2);
			ofVec2f center = (t0 + t1 + t2) / 3;
			
			int i3 = subtriangles.getNumVertices();
			subtriangles.addVertex(ofVec3f(0, 0, 1) + center);
			subtriangles.addColor(top);
			
			subtriangles.addIndex(i0);
			subtriangles.addIndex(i1);
			subtriangles.addIndex(i3);
			
			subtriangles.addIndex(i1);
			subtriangles.addIndex(i2);
			subtriangles.addIndex(i3);
			
			subtriangles.addIndex(i2);
			subtriangles.addIndex(i0);
			subtriangles.addIndex(i3);
		}
		
		subtriangles = convertFromIndices(subtriangles);
		
		for(int i = 0; i < subtriangles.getNumVertices(); i++) {
			if(subtriangles.getVertex(i).z > 0) {
				tops.push_back(i);
			}
		}
	}
	
	void update() {
		propagate();
		drawRipples();
		
		updateTops();
		buildNormals(subtriangles);
		
		light.setPosition(mouseX, mouseY, +500);
	}
	
	void updateTops() {
		vector<MapType>& cur = side ? pong : ping;
		for(int i = 0; i < tops.size(); i++) {
			ofVec3f& top = subtriangles.getVertices()[tops[i]];
			top.z = topOffset + get(cur, top.x / scaleFactor, top.y / scaleFactor);
		}
	}
	
	void disturb(int x, int y, int radius) {
		vector<MapType>& cur = side ? pong : ping;
		for(int cx = -radius; cx <= radius; cx++) {
			for(int cy = -radius; cy <= radius; cy++) {
				int xo = cx + x, yo = cy + y;
				if(ofVec2f(cx, cy).length() < radius && xo > 0 && xo < width - 1 && yo > 0 && yo < height - 1) {
					get(cur, xo, yo) -= range;
				}
			}
		}
	}
	
	void propagate() {
		side = !side;
		vector<MapType>& a = (side ? ping : pong);
		vector<MapType>& b = (side ? pong : ping);
		for(int y = 1; y < height - 1; y++) {
			for(int x = 1; x < width - 1; x++) {
				MapType sum = 
				get(a, x - 1, y) +
				get(a, x + 1, y) +
				get(a, x, y - 1) +
				get(a, x, y + 1);
				sum /= 2;
				sum -= get(b, x, y);
				sum -= sum / density;
				get(b, x, y) = sum;
			}
		}
	}
	
	void drawRipples() {		
		vector<MapType>& cur = side ? ping : pong;
		for(int i = 0; i < n; i++) {
			out.getPixels()[i] = ofMap(cur[i], -range, range, 0, 255, true);
		}
		out.update();
	}
	
	void draw() {
		ofBackground(0);
		
		glDisable(GL_DEPTH_TEST);
		ofSetColor(255);
		out.draw(0, 0, ofGetWidth(), ofGetHeight());
		
		cam.begin();
		ofRotateX(-45);
		ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
		ofSetColor(255);
		ofEnableLighting();
		glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
		subtriangles.draw();
    glDisable(GL_CULL_FACE);
		ofDisableLighting();
		
		ofSetColor(0);
		ofTranslate(0, 0, 1);
		ofSetLineWidth(2);
		triangles.drawWireframe();
		cam.end();
	}
	
	void mouseMoved(int x, int y) {
		disturb(x / scaleFactor, y / scaleFactor, 8);
	}
};

int main() {
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth");
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
