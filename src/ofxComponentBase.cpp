#include "ofxComponentBase.h"

namespace ofxComponent {

	ofxComponentBase::ofxComponentBase() {
	}


	ofxComponentBase::~ofxComponentBase() {
	}

	void ofxComponentBase::setup() {
		onSetup();
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->setup();
		}
	}

	void ofxComponentBase::update(ofEventArgs &args) {
		if (!isActive) return;

		onUpdate();
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->update(args);
		}
		postUpdate();
	}

	void ofxComponentBase::draw(ofEventArgs &args) {
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

	void ofxComponentBase::exit(ofEventArgs &args) {
		onExit();
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->exit(args);
		}
	}

	void ofxComponentBase::setActive(bool active) {
		if (isActive == active) return;
		isActive = active;
		onActiveChanged();
	}

	void ofxComponentBase::keyPressed(ofKeyEventArgs &key) {
		if (!isActive) return;

		onKeyPressed(key);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->keyPressed(key);
		}
	}

	void ofxComponentBase::keyReleased(ofKeyEventArgs &key) {
		if (!isActive) return;

		onKeyReleased(key);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->keyReleased(key);
		}
	}

	void ofxComponentBase::mouseMoved(ofMouseEventArgs &mouse) {
		if (!isActive) return;

		onMouseMoved(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->mouseMoved(mouse);
		}
	}

	void ofxComponentBase::mousePressed(ofMouseEventArgs &mouse) {
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

	void ofxComponentBase::mouseDragged(ofMouseEventArgs &mouse) {
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

	void ofxComponentBase::mouseReleased(ofMouseEventArgs &mouse) {
		if (!isActive) return;

		if (dragging) dragging = false;

		onMouseReleased(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->mouseReleased(mouse);
		}
	}

	void ofxComponentBase::dragEvent(ofDragInfo &dragInfo) {
		onDragEvent(dragInfo);

		for (int i = 0; i < children.size(); ++i) {
			auto &c = children[i];
			c->dragEvent(dragInfo);
		}
	}

	ofRectangle ofxComponentBase::getRect() {
		return rect;
	}

	ofRectangle ofxComponentBase::getGlobalRect() {
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

	ofVec2f ofxComponentBase::getPos() {
		return ofVec2f(rect.x, rect.y);
	}

	ofVec2f ofxComponentBase::getGlobalPos() {
		return localToGlobalPos(getPos());
	}

	float ofxComponentBase::getParentWidth() {
		if (parent == nullptr) {
			return ofGetWidth();
		}
		else {
			return parent->getWidth();
		}
	}

	float ofxComponentBase::getParentHeight() {
		if (parent == nullptr) {
			return ofGetHeight();
		}
		else {
			return parent->getHeight();
		}
	}

	float ofxComponentBase::getWidth() {
		return rect.width;
	}

	float ofxComponentBase::getHeight() {
		return rect.height;
	}

	float ofxComponentBase::getScale() {
		return scale;
	}

	float ofxComponentBase::getGlobalScale() {
		auto scaleVec = globalMatrix.getScale();
		return scaleVec.x;
	}

	float ofxComponentBase::getRotation() {
		return rotation;
	}

	float ofxComponentBase::getGlobalRotation() {
		auto quaternion = globalMatrix.getRotate();
		auto rotationVec = quaternion.getEuler();
		return rotationVec.z;
	}

	void ofxComponentBase::setRect(ofRectangle _rect) {
		rect = _rect;
		updateMatrix();
	}

	void ofxComponentBase::setPos(float x, float y) {
		setRect(ofRectangle(x, y, rect.width, rect.height));
	}

	void ofxComponentBase::setPos(ofVec2f _pos) {
		setPos(_pos.x, _pos.y);
	}

	void ofxComponentBase::setGlobalPos(float x, float y) {
		setGlobalPos(ofVec2f(x, y));
	}

	void ofxComponentBase::setGlobalPos(ofVec2f _gPos) {
		setPos(globalToLocalPos(_gPos));
	}

	void ofxComponentBase::setCenterPos(float x, float y) {
		setRect(ofRectangle(x - rect.width / 2, y - rect.height / 2, rect.width, rect.height));
	}

	void ofxComponentBase::setCenterPos(ofVec2f pos) {
		setCenterPos(pos.x, pos.y);
	}

	void ofxComponentBase::setWidth(float _width) {
		setRect(ofRectangle(rect.x, rect.y, _width, rect.height));
	}

	void ofxComponentBase::setHeight(float _height) {
		setRect(ofRectangle(rect.x, rect.y, rect.width, _height));
	}

	void ofxComponentBase::setScale(float _scale) {
		scale = _scale;
		updateMatrix();
	}

	void ofxComponentBase::setRotation(float _rotation) {
		rotation = _rotation;
		updateMatrix();
	}

	ofVec2f ofxComponentBase::globalToLocalPos(ofVec2f _globalPos) {
		return globalMatrixInverse.preMult(ofVec3f(_globalPos));
	}

	ofVec2f ofxComponentBase::localToGlobalPos(ofVec2f _localPos) {
		return globalMatrix.preMult(ofVec3f(_localPos));
	}

	float ofxComponentBase::getMouseX() {
		return getMousePos().x;
	}

	float ofxComponentBase::getMouseY() {
		return getMousePos().y;
	}

	ofVec2f ofxComponentBase::getMousePos() {
		return globalToLocalPos(ofVec2f(ofGetMouseX(), ofGetMouseY()));
	}

	ofVec2f ofxComponentBase::getPreviousMousePos() {
		return globalToLocalPos(ofVec2f(ofGetPreviousMouseX(), ofGetPreviousMouseY()));
	}

	ofMatrix4x4 ofxComponentBase::getLocalMatrix() {
		return localMatrix;
	}

	ofMatrix4x4 ofxComponentBase::getLocalMatrixInverse() {
		return localMatrixInverse;
	}

	ofMatrix4x4 ofxComponentBase::getGlobalMatrix() {
		return globalMatrix;
	}

	ofMatrix4x4 ofxComponentBase::getGlobalMatrixInverse() {
		return globalMatrixInverse;
	}

	void ofxComponentBase::setDraggable(bool _draggable) {
		draggable = _draggable;
		if (!draggable) dragging = false;
	}

	bool ofxComponentBase::getDraggable() {
		return draggable;
	}

	bool ofxComponentBase::getDragging() {
		return dragging;
	}

	void ofxComponentBase::setParent(shared_ptr<ofxComponentBase>  _parent) {
		if (parent == _parent) return;

		if (parent != nullptr) {
			parent->removeChild(shared_from_this());
		}

		parent = _parent;

		if (parent != nullptr) {
			parent->addChild(shared_from_this());
		}
	}

	void ofxComponentBase::removeParent() {
		if (parent != nullptr) {
			parent->removeChild(shared_from_this());
			parent = nullptr;
		}
	}

	void ofxComponentBase::addChild(shared_ptr<ofxComponentBase>  _child) {
		// insert to back
		insertChild(_child, children.size());
	}

	void ofxComponentBase::insertChild(shared_ptr<ofxComponentBase>  _child, int index) {
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

			_child->setParent(shared_from_this());
		}
	}

	void ofxComponentBase::removeChild(shared_ptr<ofxComponentBase>  _child) {
		for (int i = 0; i < children.size(); ++i) {
			auto c = children[i];
			if (c == _child) {
				_child->parent = nullptr;
				children.erase(children.begin() + i);
				break;
			}
		}
	}
	
	shared_ptr<ofxComponentBase>  ofxComponentBase::getChild(int i) {
		if (i < 0 || children.size() <= i) {
			return nullptr;
		}
		return children[i];
	}

	void ofxComponentBase::updateMatrix() {
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

	void ofxComponentBase::updateGlobalMatrix() {
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