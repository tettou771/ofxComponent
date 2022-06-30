#pragma once

#include "ofMain.h"
#include "ofxComponent.h"

using namespace ofxComponent;

// position coordination
class YellowComponent : public ofxComponentBase {
	void onStart() override {
		setRect(ofRectangle(50, 100, 400, 300));
		setRotation(30);
		setScale(0.7);
	}
	void onDraw() override {
		ofSetColor(ofColor::yellow);
		ofDrawRectangle(0, 0, getWidth(), getHeight()); // getWidth(), getHeight() is Component's member

		// draw cursor pos that is relative coordination.
		if (0 < getMouseX() && getMouseX() < getWidth()
			&& 0 < getMouseY() && getMouseY() < getHeight()) {
			ofSetColor(ofColor::darkGray);
			ofDrawCircle(getMouseX(), getMouseY(), 10);

			// draw local and global position
			ofVec2f localMouse = getMousePos();
			stringstream ss;
			ss << "local (" << localMouse.x << ", " << localMouse.y << ")" << "\nglobal (" << ofGetMouseX() << ", " << ofGetMouseY() << ")";
			ofDrawBitmapString(ss.str(), getMouseX() + 20, getMouseY());

			// if you wanna get global pos from local,
			// use this: localToGlobalPos(localPosition);
		}
	}
};

// draggable component
class BlueComponent : public ofxComponentBase {
	void onStart() override {
        setMovable(true);
		setRect(ofRectangle(20, 20, 100, 100));
	}
	void onDraw() override {
		// light color when dragging
		if (getMoving()) {
			ofSetColor(ofColor(100, 100, 255));
		}
		else {
			ofSetColor(ofColor::blue);
		}

		ofDrawRectangle(0, 0, getWidth(), getHeight());

		ofSetColor(ofColor::white);
		ofDrawBitmapString("Drag\nme", 5, 20);
	}
};

// parent compornent demo
class RedComponent : public ofxComponentBase {
	void onStart() override {
		blueInRed = make_shared<BlueComponent>();
		addChild(blueInRed);
        setMovable(true);
        
        // children will draw only in this component area (with FBO)
        setConstrain(true);
	}
	void onUpdate() override {
		ofLog() << "RedComponent onUpdate()";
	}
	void postUpdate() override {
		ofLog() << "RedComponent postUpdate()";
	}
	void onDraw() override {
		ofSetColor(ofColor::red);
		ofDrawRectangle(0, 0, getWidth(), getHeight());

		ofSetColor(ofColor::white);
		for (int y = 10; y < getHeight(); y += 50) {
			ofDrawBitmapString("Draw BEFORE child components.", 10, y);
		}
	}
	void postDraw() override {
		ofSetColor(ofColor::white);
		for (int y = 25; y < getHeight(); y += 50) {
			ofDrawBitmapString("Draw AFTER child components.", 100, y);
		}
	}
	void onExit() override {
		ofLog() << "RedComponent onExit()";
	}

private:
	shared_ptr<YellowComponent> yellowInRed;
	shared_ptr<BlueComponent> blueInRed;
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	shared_ptr<ofxComponentManager> manager;
	shared_ptr<RedComponent> red;
	shared_ptr<YellowComponent> yellow;
	shared_ptr<BlueComponent> blue;
};
