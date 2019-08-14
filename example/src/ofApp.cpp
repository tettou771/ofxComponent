#include "ofApp.h"

using namespace ofxComponent;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundAuto(true);

	// Set drawing area
	manager.setRect(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));

	/*
	// Set pos, width, height
	// serRect() and These meshod is same behaviour
	manager.setPos(0, 0);
	manager.setWidth(ofGetWidth());
	manager.setHeight(ofGetHeight());
	*/

	red.setRect(ofRectangle(500, 100, 400, 400));

	// Add children
	// draw() and update() etc... is sorted by this order
	manager.addChild(&red);
	manager.addChild(&yellow);
	manager.addChild(&blue);

	// Setup all objects
	// Please call this after children add
	manager.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
