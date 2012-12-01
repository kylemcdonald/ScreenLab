#include "ofAppGlutWindow.h"
#include "ofMain.h"
#include "ofApp.h"

int main() {
	ofAppGlutWindow window;
	window.setGlutDisplayString("rgba double samples>=4 depth");
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
