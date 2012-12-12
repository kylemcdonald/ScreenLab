#pragma once

#include "ofMain.h"
#include "ofxVVVV.h"
#include "ofApp.h"

#define NODE_CLASS testNode

using namespace ofxVVVV;

class testNode : public Node{

	public:
		testNode() { }
		void setupPins(Host &);
		string getName() { return "Octave Test"; };

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		Pin testInput1, testInput2;
		Pin testOutput;

		ofApp octave;
};

#include "ofxVVVV/Factory.h"