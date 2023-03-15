#pragma once

#include "ofMain.h"
#include "ofxComponent.h"

using namespace ofxComponent;

// position coordination
class YellowComponent : public ofxComponentBase {
	void onDraw() override {
		ofSetColor(255, 204, 102); // yellow
		ofDrawRectangle(0, 0, getWidth(), getHeight()); // getWidth(), getHeight() is Component's member

		// draw cursor pos that is relative coordination.
		if (0 < getMouseX() && getMouseX() < getWidth()
			&& 0 < getMouseY() && getMouseY() < getHeight()) {
			ofSetColor(51, 51, 51); // dark gray
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
        setWidth(100);
        setHeight(100);
        // position is relative
        setPos(ofVec2f(20, 20));
	}
	void onDraw() override {
		// light color when dragging
		if (getMoving()) {
			ofSetColor(102, 153, 204, 150); // blue transparent
		}
		else {
            ofSetColor(102, 153, 204); // blue
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
        ofSetColor(204, 102, 102); // red
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

// button demo
class PurpleButtonComponent : public ofxComponentBase {
public:
    void onDraw() override {
        ofSetColor(153, 102, 204); // purple
        ofDrawRectangle(0, 0, getWidth(), getHeight());
        ofSetColor(ofColor::white);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawRectangle(0, 0, getWidth(), getHeight());

        // draw text with status
        ofSetColor(ofColor::white);
        if (isMouseOver()) {
            // mouse pressing
            if (isMousePressedOverComponent()) {
                ofDrawBitmapString("Press", 10, 20);
            }
            // mouse hover
            else {
                ofDrawBitmapString("Hover", 10, 20);
            }
        }
        // No mouse
        else {
            ofDrawBitmapString(name, 10, 20);
        }
    }
    
    // pressed event
    void onMousePressed(ofMouseEventArgs& mouse) override {
        // if on mouse
        if (isMouseOver()) {
            ofLogNotice("PurpleButtonComponent") << name << ": Mouse pressed !";
        }
    }
    
    string name;
};

// UI with button demo
class GreenComponent : public ofxComponentBase {
public:
    void onStart() override {
        // make purple buttons
        
        // overrap button
        int numOverrapBtns = 3;
        for (int i=0; i<numOverrapBtns; ++i) {
        }

        // button list
        int numListBtns = 3;
        for (int i=0; i<numListBtns; ++i) {
            auto btn = make_shared<PurpleButtonComponent>();
            btn->setWidth(200);
            btn->setHeight(30);
            btn->setPos(ofVec2f(10, 10 + i * 40));
            btn->name = "Button" + ofToString(i);
            addChild(btn);
            
            // set button event listener
            ofAddListener(btn->mousePressedOverComponentEvents, this, &GreenComponent::onButtonPressed);
        }

        // overrap button
        auto overrapBtn = make_shared<PurpleButtonComponent>();
        overrapBtn->setWidth(150);
        overrapBtn->setHeight(100);
        overrapBtn->setPos(ofVec2f(120, 60));
        overrapBtn->name = "Overrap Button";
        addChild(overrapBtn);
        ofAddListener(overrapBtn->mousePressedOverComponentEvents, this, &GreenComponent::onButtonPressed);
    }
    
    void onDraw() override {
        ofSetColor(102, 204, 153); // green
        ofDrawRectangle(0, 0, getWidth(), getHeight());
    }
    
    // button event
    void onButtonPressed() {
        ofLogNotice("GreenComponent") << "Button pressed event";
    }
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
};
