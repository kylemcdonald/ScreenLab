#include "ofAppGlutWindow.h"
#include "ofMain.h"

#include "Stepping.h"

class ofApp : public ofBaseApp {
public:
	Stepping stepping;

	void setup() {
		ofSetVerticalSync(true);
	}
	
	void draw() {
		ofBackground(0);
		ofNoFill();
		
		stepping.update();
		ofCircle(stepping.footstep, 10);
	}
};

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 512, 512, OF_WINDOW);
	ofRunApp(new ofApp());
}
