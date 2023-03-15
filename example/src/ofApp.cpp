#include "ofApp.h"

using namespace ofxComponent;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundAuto(true);

	// Set drawing area
	manager = make_shared<ofxComponentManager>();
	manager->setRect(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));

	/*
	// Set pos, width, height
	// serRect() and These meshod is same behaviour
	manager.setPos(0, 0);
	manager.setWidth(ofGetWidth());
	manager.setHeight(ofGetHeight());
	*/

	// make shared pointer instance
	auto red = make_shared<RedComponent>();
    // set position and size with rectangle
    red->setRect(ofRectangle(150, 20, 400, 400));
    
	auto yellow = make_shared<YellowComponent>();
    // another way, set size, position, rotation and scale individualy
    yellow->setWidth(400);
    yellow->setHeight(300);
    yellow->setPos(ofVec2f(600, 50));
    yellow->setRotation(30);
    yellow->setScale(0.7);

	auto blue = make_shared<BlueComponent>();
    // blue position is set when BlueComponent::onStart()
    
    auto green = make_shared<GreenComponent>();
    green->setRect(ofRectangle(50, 400, 280, 180));

	// Add children
	// draw() and update() etc... is sorted by this order
	manager->addChild(red);
	manager->addChild(yellow);
	manager->addChild(blue);
    manager->addChild(green);

	// Setup all objects
	// Please call this after children add
	manager->setup();
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
