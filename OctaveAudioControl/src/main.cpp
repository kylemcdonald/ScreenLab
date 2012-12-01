#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "ofxNetwork.h"

float pixelsPerMeter = 100;
float eyeLevel = 0;//1.8;

template <class T>
void writeRaw(ostringstream& out, T data) {
	out.write((char*) &data, sizeof(data)); 
}

class ofApp : public ofBaseApp{
public:
	ofxUDPManager udpConnection;
	vector<ofVec2f> positions;
	
	void keyPressed(int key) {
		if(key == 'r') {
			rewind(0);
			rewind(1);
			rewind(2);
			rewind(3);
		}
	}
	
	void rewind(int index) {
		ostringstream msg(ostringstream::binary);
		
		const static int
		REWIND = 19,
		devices = 0,
		loudspeakerCount = 0,
		sourceCount = 1;
		
		writeRaw(msg, REWIND); //updPacket.command
		writeRaw(msg, devices); //updPacket.devices
		writeRaw(msg, loudspeakerCount); //updPacket.loudspeakerCount
		writeRaw(msg, sourceCount); //updPacket.sourceCount
		
		float amplitude = 1;
		bool on = true;
		bool loop = false;
		int renderMethod = 6;
		float x = 0, y = 0, z = 0;
		
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
		ofSetFrameRate(30);
		
		positions.resize(4);
		
		udpConnection.Create();
		udpConnection.Connect("146.87.67.40", 201);
		udpConnection.SetNonBlocking(true);
	}
	
	void update(){
		float t = .1 * ofGetElapsedTimef();
		float theta = ofGetElapsedTimef() * 15;
		if(ofGetMousePressed()) {
			theta = ofMap(mouseX, 0, ofGetWidth(), 0, 360);
		}
		float thetaOffset = 360. / positions.size();
		for(int i = 0; i < positions.size(); i++) {
			float radius = ofMap(i % 2, 0, 1, 1.2, 1.8);
			positions[i] = ofVec2f(radius, 0).rotate(theta + thetaOffset * i);
			editSource(i, positions[i].x, positions[i].y, eyeLevel);
		}
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
		for(int i = 0; i < positions.size(); i++) {
			ofSetColor(ofColor::red);
			ofCircle(positions[i], .1);
			ofSetColor(255);
			ofDrawBitmapString(ofToString(i), positions[i]);
		}
		ofPopStyle();
	}
};

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800, 800, OF_WINDOW);
	ofRunApp(new ofApp());
}
