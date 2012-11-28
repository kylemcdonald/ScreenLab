#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "ofxNetwork.h"

float pixelsPerMeter = 100;
float eyeLevel = 1.8;

template <class T>
void writeRaw(ostringstream& out, T data) {
	out.write((char*) &data, sizeof(data)); 
}

class ofApp : public ofBaseApp{
public:
	ofxUDPManager udpConnection;
	ofVec2f position;
	
	void editSource(int index, float x, float y, float z) {
		ostringstream msg(ostringstream::binary);
		
		const static int
		EDIT_SOURCE = 16,
		devices = 0,
		loudspeakerCount = 0,
		sourceCount = 1;
		
		writeRaw(msg, EDIT_SOURCE); //updPacket.command
		writeRaw(msg, devices); //updPacket.devices
		writeRaw(msg, loudspeakerCount); //updPacket.loudspeakerCount
		writeRaw(msg, sourceCount); //updPacket.sourceCount
		
		float amplitude = 1;
		bool on = true;
		bool loop = false;
		int renderMethod = 6;
		
		writeRaw(msg, index); // int 0-3
		writeRaw(msg, amplitude); // float 1.0
		writeRaw(msg, on); // bool true
		writeRaw(msg, loop); // bool false
		writeRaw(msg, x); // float
		writeRaw(msg, y); // float
		writeRaw(msg, z); // float
		writeRaw(msg, renderMethod); // int 0
		writeRaw(msg, '\0'); // empty null-terminated string
		
		string msgString = msg.str();
		udpConnection.Send(msgString.c_str(), msgString.length());
	}
	
	void setup(){
		ofSetVerticalSync(true);
		ofSetFrameRate(60);
		
		udpConnection.Create();
		udpConnection.Connect("146.87.67.72", 201);
		udpConnection.SetNonBlocking(true);
	}
	
	void update(){
		float t = .1 * ofGetElapsedTimef();
		//position.set(ofNoise(t, 0), ofNoise(0, t));
		position = ofVec2f(2, 0).rotate(ofGetElapsedTimef() * 15);
		
		editSource(0, position.x, position.y, eyeLevel);
	}
	
	void draw(){
		ofBackground(0);
		ofNoFill();
		
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofScale(pixelsPerMeter, -pixelsPerMeter, pixelsPerMeter);
		
		ofDrawGrid(4, 4, true, false, false, true);
		
		ofPushStyle();
		ofPushMatrix();
		ofSetCircleResolution(8);
		ofRotate(360. / (8 * 2));
		ofCircle(0, 0, 3.397);
		ofPopStyle();
		ofPopMatrix();
		
		ofPushStyle();
		ofSetColor(ofColor::red);
		ofCircle(position, .1);
		ofPopStyle();
	}
};

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800, 800, OF_WINDOW);
	ofRunApp(new ofApp());
}
