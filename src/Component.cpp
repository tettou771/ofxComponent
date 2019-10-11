#include "Component.h"

namespace ofxComponent {

	Component::Component() {
	}


	Component::~Component() {
		/*
		if (parent != nullptr) {
			parent->removeChild(this);
		}
		*/
	}

	void Component::setup() {
		onSetup();
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->setup();
		}
	}

	void Component::update(ofEventArgs &args) {
		if (!isActive) return;

		onUpdate();
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->update(args);
		}
		postUpdate();
	}

	void Component::draw(ofEventArgs &args) {
		if (!isActive) return;

		ofPushMatrix();
		ofMultMatrix(getLocalMatrix());

		ofPushStyle();
		onDraw();
		ofPopStyle();

		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->draw(args);
		}

		ofPushStyle();
		postDraw();
		ofPopStyle();

		ofPopMatrix();
	}

	void Component::exit(ofEventArgs &args) {
		onExit();
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->exit(args);
		}
	}

	void Component::setActive(bool active) {
		if (isActive == active) return;
		isActive = active;
		onActiveChanged();
	}

	void Component::keyPressed(ofKeyEventArgs &key) {
		if (!isActive) return;

		onKeyPressed(key);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->keyPressed(key);
		}
	}

	void Component::keyReleased(ofKeyEventArgs &key) {
		if (!isActive) return;

		onKeyReleased(key);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->keyReleased(key);
		}
	}

	void Component::mouseMoved(ofMouseEventArgs &mouse) {
		if (!isActive) return;

		onMouseMoved(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->mouseMoved(mouse);
		}
	}

	void Component::mousePressed(ofMouseEventArgs &mouse) {
		if (!isActive) return;

		if (draggable && getGlobalRect().inside(ofGetMouseX(), ofGetMouseY())) {
			dragging = true;
		}

		onMousePressed(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->mousePressed(mouse);
		}
	}

	void Component::mouseDragged(ofMouseEventArgs &mouse) {
		if (!isActive) return;

		if (dragging) {
			ofVec2f move = getMousePos() - getPreviousMousePos();
			setPos(getPos() + move);
		}

		onMouseDragged(mouse);
		for (auto &c : children) {
			c->mouseDragged(mouse);
		}
	}

	void Component::mouseReleased(ofMouseEventArgs &mouse) {
		if (!isActive) return;

		if (dragging) dragging = false;

		onMouseReleased(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->mouseReleased(mouse);
		}
	}

	void Component::dragEvent(ofDragInfo &dragInfo) {
		onDragEvent(dragInfo);

		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->dragEvent(dragInfo);
		}
	}

	ofRectangle Component::getRect() {
		return rect;
	}

	ofRectangle Component::getGlobalRect() {
		if (parent == nullptr) {
			return rect;
		}
		else {
			ofRectangle pRect = parent->getGlobalRect();
			ofRectangle globalRect;
			globalRect.x = rect.x + pRect.x;
			globalRect.y = rect.y + pRect.y;
			globalRect.width = rect.width;
			globalRect.height = rect.height;
			return globalRect;
		}
	}

	ofVec2f Component::getPos() {
		return ofVec2f(rect.x, rect.y);
	}

	ofVec2f Component::getGlobalPos() {
		return localToGlobalPos(getPos());
	}

	float Component::getParentWidth() {
		if (parent == nullptr) {
			return ofGetWidth();
		}
		else {
			return parent->getWidth();
		}
	}

	float Component::getParentHeight() {
		if (parent == nullptr) {
			return ofGetHeight();
		}
		else {
			return parent->getHeight();
		}
	}

	float Component::getWidth() {
		return rect.width;
	}

	float Component::getHeight() {
		return rect.height;
	}

	float Component::getScale() {
		return scale;
	}

	float Component::getRotation() {
		return rotation;
	}

	void Component::setRect(ofRectangle _rect) {
		rect = _rect;
		updateMatrix();
	}

	void Component::setPos(float x, float y) {
		setRect(ofRectangle(x, y, rect.width, rect.height));
	}

	void Component::setPos(ofVec2f _pos) {
		setPos(_pos.x, _pos.y);
	}

	void Component::setCenterPos(float x, float y) {
		setRect(ofRectangle(x - rect.width / 2, y - rect.height / 2, rect.width, rect.height));
	}

	void Component::setCenterPos(ofVec2f pos) {
		setCenterPos(pos.x, pos.y);
	}

	void Component::setWidth(float _width) {
		setRect(ofRectangle(rect.x, rect.y, _width, rect.height));
	}

	void Component::setHeight(float _height) {
		setRect(ofRectangle(rect.x, rect.y, rect.width, _height));
	}

	void Component::setScale(float _scale) {
		scale = _scale;
		updateMatrix();
	}

	void Component::setRotation(float _rotation) {
		rotation = _rotation;
		updateMatrix();
	}
	
	ofVec2f Component::globalToLocalPos(ofVec2f _globalPos) {
		return globalMatrixInverse.preMult(ofVec3f(_globalPos));
	}

	ofVec2f Component::localToGlobalPos(ofVec2f _localPos) {
		return globalMatrix.preMult(ofVec3f(_localPos));
	}

	float Component::getMouseX() {
		return getMousePos().x;
	}

	float Component::getMouseY() {
		return getMousePos().y;
	}

	ofVec2f Component::getMousePos() {
		return globalToLocalPos(ofVec2f(ofGetMouseX(), ofGetMouseY()));
	}

	ofVec2f Component::getPreviousMousePos() {
		return globalToLocalPos(ofVec2f(ofGetPreviousMouseX(), ofGetPreviousMouseY()));
	}

	ofMatrix4x4 Component::getLocalMatrix() {
		return localMatrix;
	}

	ofMatrix4x4 Component::getLocalMatrixInverse() {
		return localMatrixInverse;
	}

	ofMatrix4x4 Component::getGlobalMatrix() {
		return globalMatrix;
	}

	ofMatrix4x4 Component::getGlobalMatrixInverse() {
		return globalMatrixInverse;
	}

	void Component::setDraggable(bool _draggable) {
		draggable = _draggable;
		if (!draggable) dragging = false;
	}

	bool Component::getDraggable() {
		return draggable;
	}

	bool Component::getDragging() {
		return dragging;
	}

	void Component::setParent(Component * _parent) {
		if (parent == _parent) return;

		if (parent != nullptr) {
			parent->removeChild(this);
		}

		parent = _parent;

		if (parent != nullptr) {
			parent->addChild(this);
		}
	}

	void Component::removeParent() {
		if (parent != nullptr) {
			parent->removeChild(this);
			parent = nullptr;
		}
	}

	void Component::addChild(Component * _child) {
		// insert to back
		insertChild(_child, children.size());
	}

	void Component::insertChild(Component * _child, int index) {
		bool alreadyListed = false;

		for (auto &c : children) {
			if (c == _child) {
				alreadyListed = true;
				break;
			}
		}

		if (!alreadyListed) {
			// add to back if index is over
			if (children.size() <= index) {
				children.push_back(_child);
			}

			else if (0 <= index) {
				children.insert(children.begin() + index, _child);
			}

			else {
				children.insert(children.begin(), _child);
			}

			_child->setParent(this);
		}
	}

	void Component::removeChild(Component * _child) {
		for (int i = 0; i < children.size(); ++i) {
			auto c = children[i];
			if (c == _child) {
				children.erase(children.begin() + i);
				_child->removeParent();
				break;
			}
		}
	}
	
	Component * Component::getChild(int i) {
		if (i < 0 || children.size() <= i) {
			return nullptr;
		}
		return children[i];
	}

	void Component::updateMatrix() {
		/*
		localMatrix.translate(ofVec3f(rect.x - rect.width / 2, rect.y - rect.height / 2, 0));
		localMatrix.rotateRad(rotation * DEG_TO_RAD, 0, 0, 1);
		localMatrix.translate(ofVec3f(rect.width / 2, rect.height / 2, 0));
		localMatrix.scale(ofVec3f(scale, scale, scale));
		*/

		// make local
		localMatrix = ofMatrix4x4();
		localMatrix.translate(ofVec3f(-rect.width / 2, -rect.height / 2, 0));
		localMatrix.rotateRad(rotation * DEG_TO_RAD, 0, 0, 1);
		localMatrix.scale(ofVec3f(scale, scale, scale));
		localMatrix.translate(ofVec3f(rect.x, rect.y, 0));
		localMatrix.translate(ofVec3f(rect.width / 2, rect.height / 2, 0));

		localMatrixInverse = localMatrix.getInverse();

		updateGlobalMatrix();
	}

	void Component::updateGlobalMatrix() {
		// make global
		if (parent == nullptr) {
			globalMatrix = localMatrix;
		}
		else {
			globalMatrix = localMatrix * parent->getGlobalMatrix();
		}

		globalMatrixInverse = globalMatrix.getInverse();

		for (auto &c : children) {
			c->updateGlobalMatrix();
		}
	}
}