#include "ofxComponentManager.h"


namespace ofxComponent {
	ofxComponentManager::ofxComponentManager() {
	}


	ofxComponentManager::~ofxComponentManager() {
		ofRemoveListener(ofEvents().update, this, &ofxComponentManager::update, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().draw, this, &ofxComponentManager::draw, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().exit, this, &ofxComponentManager::exit, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().keyPressed, this, &ofxComponentManager::keyPressed, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().keyReleased, this, &ofxComponentManager::keyReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mouseMoved, this, &ofxComponentManager::mouseMoved, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mousePressed, this, &ofxComponentManager::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mouseDragged, this, &ofxComponentManager::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mouseReleased, this, &ofxComponentManager::mouseReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().fileDragEvent, this, &ofxComponentManager::dragEvent, OF_EVENT_ORDER_BEFORE_APP);
	}

	void ofxComponentManager::onSetup() {
		ofAddListener(ofEvents().update, this, &ofxComponentManager::update, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().draw, this, &ofxComponentManager::draw, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().exit, this, &ofxComponentManager::exit, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().keyPressed, this, &ofxComponentManager::keyPressed, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().keyReleased, this, &ofxComponentManager::keyReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mouseMoved, this, &ofxComponentManager::mouseMoved, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mousePressed, this, &ofxComponentManager::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mouseDragged, this, &ofxComponentManager::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mouseReleased, this, &ofxComponentManager::mouseReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().fileDragEvent, this, &ofxComponentManager::dragEvent, OF_EVENT_ORDER_BEFORE_APP);
	}

	void ofxComponentManager::update(ofEventArgs &args) {
		ofxComponentBase::update(args);
	}
	void ofxComponentManager::draw(ofEventArgs &args) {
		ofxComponentBase::draw(args);
	}
	void ofxComponentManager::exit(ofEventArgs &args) {
		ofxComponentBase::exit(args);
	}
	void ofxComponentManager::keyPressed(ofKeyEventArgs &key) {
		ofxComponentBase::keyPressed(key);
	}
	void ofxComponentManager::keyReleased(ofKeyEventArgs &key) {
		ofxComponentBase::keyReleased(key);
	}
	void ofxComponentManager::mouseMoved(ofMouseEventArgs &mouse) {
		ofxComponentBase::mouseMoved(mouse);
	}
	void ofxComponentManager::mousePressed(ofMouseEventArgs &mouse) {
		ofxComponentBase::mousePressed(mouse);
	}
	void ofxComponentManager::mouseDragged(ofMouseEventArgs &mouse) {
		ofxComponentBase::mouseDragged(mouse);
	}
	void ofxComponentManager::mouseReleased(ofMouseEventArgs &mouse) {
		ofxComponentBase::mouseReleased(mouse);
	}
	void ofxComponentManager::dragEvent(ofDragInfo &dragInfo) {
		ofxComponentBase::dragEvent(dragInfo);
	}
}