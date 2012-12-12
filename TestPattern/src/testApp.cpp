#include "testApp.h"

//--------------------------------------------------------------
void testNode::setupPins(Host& host){
	testInput1.setName("Input");
	testInput2.setName("Another input");
	testOutput.setName("Output");
	
	host.addInputValuePin(testInput1);
	host.addInputValuePin(testInput2);
	host.addOutputValuePin(testOutput);
}

//--------------------------------------------------------------
void testNode::setup(){
	//system("c:\\windows\\notepad.exe");
	octave.setup();
}

//--------------------------------------------------------------
void testNode::update(){
	octave.update();
}

//--------------------------------------------------------------
void testNode::draw(){
	octave.draw();
}

//--------------------------------------------------------------
void testNode::keyPressed(int key){

}

//--------------------------------------------------------------
void testNode::keyReleased(int key){

}

//--------------------------------------------------------------
void testNode::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testNode::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testNode::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testNode::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testNode::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testNode::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testNode::dragEvent(ofDragInfo dragInfo){ 

}