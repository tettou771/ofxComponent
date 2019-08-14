#include "Component.h"

namespace ofxComponent {

	Component::Component() {
	}


	Component::~Component() {
	}

	void Component::setup() {
		onSetup();
		for (auto &c : children) {
			c->setup();
		}
	}

	void Component::draw(ofEventArgs &args) {
		ofPushMatrix();
		ofMultMatrix(getLocalMatrix());

		ofPushStyle();
		onDraw();
		ofPopStyle();

		for (auto &c : children) {
			c->draw(args);
		}

		ofPushStyle();
		postDraw();
		ofPopStyle();

		ofPopMatrix();
	}

	void Component::update(ofEventArgs &args) {
		onUpdate();
		for (auto &c : children) {
			c->update(args);
		}
		postUpdate();
	}

	void Component::exit(ofEventArgs &args) {
		onExit();
		for (auto &c : children) {
			c->exit(args);
		}
	}

	void Component::keyPressed(ofKeyEventArgs &key) {
		onKeyPressed(key);
		for (auto &c : children) {
			c->keyPressed(key);
		}
	}

	void Component::keyReleased(ofKeyEventArgs &key) {
		onKeyReleased(key);
		for (auto &c : children) {
			c->keyPressed(key);
		}
	}

	void Component::mouseMoved(ofMouseEventArgs &mouse) {
		onMouseMoved(mouse);
		for (auto &c : children) {
			c->mouseMoved(mouse);
		}
	}

	void Component::mousePressed(ofMouseEventArgs &mouse) {
		if (draggable && getRectGlobal().inside(ofGetMouseX(), ofGetMouseY())) {
			dragging = true;
		}

		onMousePressed(mouse);
		for (auto &c : children) {
			c->mousePressed(mouse);
		}
	}

	void Component::mouseDragged(ofMouseEventArgs &mouse) {
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
		if (dragging) dragging = false;

		onMouseReleased(mouse);
		for (auto &c : children) {
			c->mouseReleased(mouse);
		}
	}

	ofRectangle Component::getRect() {
		return rect;
	}

	ofRectangle Component::getRectGlobal() {
		if (parent == nullptr) {
			return rect;
		}
		else {
			ofRectangle pRect = parent->getRectGlobal();
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
		parent = _parent;
	}

	void Component::removeParent() {
		parent = nullptr;
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