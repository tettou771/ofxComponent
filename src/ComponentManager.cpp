#include "ComponentManager.h"


namespace ofxComponent {
	ComponentManager::ComponentManager() {
	}


	ComponentManager::~ComponentManager() {
		ofRemoveListener(ofEvents().update, this, &ComponentManager::update, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().draw, this, &ComponentManager::draw, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().exit, this, &ComponentManager::exit, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().keyPressed, this, &ComponentManager::keyPressed, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().keyReleased, this, &ComponentManager::keyReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mouseMoved, this, &ComponentManager::mouseMoved, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mousePressed, this, &ComponentManager::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mouseDragged, this, &ComponentManager::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().mouseReleased, this, &ComponentManager::mouseReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofRemoveListener(ofEvents().fileDragEvent, this, &ComponentManager::dragEvent, OF_EVENT_ORDER_BEFORE_APP);
	}

	void ComponentManager::onSetup() {
		ofAddListener(ofEvents().update, this, &ComponentManager::update, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().draw, this, &ComponentManager::draw, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().exit, this, &ComponentManager::exit, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().keyPressed, this, &ComponentManager::keyPressed, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().keyReleased, this, &ComponentManager::keyReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mouseMoved, this, &ComponentManager::mouseMoved, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mousePressed, this, &ComponentManager::mousePressed, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mouseDragged, this, &ComponentManager::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().mouseReleased, this, &ComponentManager::mouseReleased, OF_EVENT_ORDER_BEFORE_APP);
		ofAddListener(ofEvents().fileDragEvent, this, &ComponentManager::dragEvent, OF_EVENT_ORDER_BEFORE_APP);
	}

	void ComponentManager::update(ofEventArgs &args) {
		Component::update(args);
	}
	void ComponentManager::draw(ofEventArgs &args) {
		Component::draw(args);
	}
	void ComponentManager::exit(ofEventArgs &args) {
		Component::exit(args);
	}
	void ComponentManager::keyPressed(ofKeyEventArgs &key) {
		Component::keyPressed(key);
	}
	void ComponentManager::keyReleased(ofKeyEventArgs &key) {
		Component::keyReleased(key);
	}
	void ComponentManager::mouseMoved(ofMouseEventArgs &mouse) {
		Component::mouseMoved(mouse);
	}
	void ComponentManager::mousePressed(ofMouseEventArgs &mouse) {
		Component::mousePressed(mouse);
	}
	void ComponentManager::mouseDragged(ofMouseEventArgs &mouse) {
		Component::mouseDragged(mouse);
	}
	void ComponentManager::mouseReleased(ofMouseEventArgs &mouse) {
		Component::mouseReleased(mouse);
	}
	void ComponentManager::dragEvent(ofDragInfo &dragInfo) {
		Component::dragEvent(dragInfo);
	}
}