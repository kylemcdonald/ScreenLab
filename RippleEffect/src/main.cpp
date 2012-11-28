#include "ofAppGlutWindow.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	typedef float MapType;
	
	static const int scaleFactor = 4;
	static const int range = 1024;
	static const float density = 32;

	int width, height, n;
	vector<MapType> ping, pong;
	bool side;
	ofImage out;
	
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
	}
	
	void update() {
		propagate();
		drawRipples();
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
		out.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	
	void mouseMoved(int x, int y) {
		disturb(x / scaleFactor, y / scaleFactor, 3);
	}
	
};

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 256, 256, OF_WINDOW);
	ofRunApp(new ofApp());
}
