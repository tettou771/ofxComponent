#include "ofxComponentBase.h"

namespace ofxComponent {
	vector<shared_ptr<ofxComponentBase> > ofxComponentBase::destroyedComponents;

	ofxComponentBase::ofxComponentBase() {
	}

	ofxComponentBase::~ofxComponentBase() {
		for (auto t : timerFunctions) {
			delete t;
		}
	}

	void ofxComponentBase::setup() {
		onSetup();
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->setup();
		}
	}

	void ofxComponentBase::update(ofEventArgs& args) {
		if (needStartExec) start();

		// exec expired timer functions
		if (!timerFunctions.empty()) {
			float now = ofGetElapsedTimef();
			timerFunctionsMutex.lock();
			for (int i = 0; i < timerFunctions.size(); ++i) {
				auto tf = timerFunctions[i];
				if (tf->canceled) {
					delete tf;
					timerFunctions.erase(timerFunctions.begin() + i);
					--i;
				}
				else if (tf->execTime <= now) {
					tf->function();
					delete tf;
					timerFunctions.erase(timerFunctions.begin() + i);
					--i;
				}
			}
			timerFunctionsMutex.unlock();
		}

		if (!isActive || destroyed) return;

		onUpdate();
		for (int i = 0; i < children.size(); ++i) {
			children[i]->update(args);
		}
		postUpdate();
	}

	void ofxComponentBase::draw(ofEventArgs& args) {
		if (needStartExec) start();

		if (!isActive || destroyed) return;

		ofPushMatrix();
		ofMultMatrix(getLocalMatrix());

		ofPushStyle();
		onDraw();
		ofPopStyle();

		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->draw(args);
		}

		ofPushStyle();
		postDraw();
		ofPopStyle();

		ofPopMatrix();
	}

	void ofxComponentBase::exit(ofEventArgs& args) {
		onExit();
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->exit(args);
		}
	}

	void ofxComponentBase::start() {
		needStartExec = false;
		onStart();
	}

	void ofxComponentBase::setActive(bool active) {
		if (isActive == active) return;
		bool beforeGlobal = getGlobalActive();
		isActive = active;
		onActiveChanged(active);
		bool afterGlobal = getGlobalActive();

		if (afterGlobal != beforeGlobal) {
			globalActiveChanged(afterGlobal);
		}
	}

	void ofxComponentBase::globalActiveChanged(bool _globalActive) {
		onGlobalActiveChanged(_globalActive);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			if (c->getActive()) {
				c->globalActiveChanged(_globalActive);
			}
		}
	}

	void ofxComponentBase::addTimerFunction(TimerFunc func, float wait) {
		timerFunctionsMutex.lock();
		timerFunctions.push_back(new Timer(func, wait));
		timerFunctionsMutex.unlock();
	}

	void ofxComponentBase::clearTimerFunctions() {
		for (auto t : timerFunctions) {
			t->canceled = true;
		}
	}

	bool ofxComponentBase::getGlobalActive() {
		if (getActive()) {
			auto p = getParent();
			if (p) {
				return p->getGlobalActive();
			}
			else {
				return true;
			}
		}
		else {
			false;
		}
	}

	void ofxComponentBase::keyPressed(ofKeyEventArgs& key) {
		if (!isActive || !keyMouseEventEnabled) return;

		onKeyPressed(key);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->keyPressed(key);
		}
	}

	void ofxComponentBase::keyReleased(ofKeyEventArgs& key) {
		if (!isActive || !keyMouseEventEnabled) return;

		onKeyReleased(key);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->keyReleased(key);
		}
	}

	void ofxComponentBase::mouseMoved(ofMouseEventArgs& mouse) {
		if (!isActive || !keyMouseEventEnabled) return;

		onMouseMoved(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->mouseMoved(mouse);
		}
	}

	void ofxComponentBase::mousePressed(ofMouseEventArgs& mouse) {
		if (!isActive || !keyMouseEventEnabled) return;

		if (draggable && isMouseInside()) {
			dragging = true;
		}

		onMousePressed(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->mousePressed(mouse);
		}
	}

	void ofxComponentBase::mouseDragged(ofMouseEventArgs& mouse) {
		if (!isActive || !keyMouseEventEnabled) return;

		if (dragging) {
			ofVec2f move = getMousePos() - getPreviousMousePos();
			setPos(getPos() + move);
		}

		onMouseDragged(mouse);
		for (auto& c : children) {
			c->mouseDragged(mouse);
		}
	}

	void ofxComponentBase::mouseReleased(ofMouseEventArgs& mouse) {
		if (!isActive || !keyMouseEventEnabled) return;

		if (dragging) dragging = false;

		onMouseReleased(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->mouseReleased(mouse);
		}
	}

	void ofxComponentBase::mouseScrolled(ofMouseEventArgs& mouse) {
		if (!isActive || !keyMouseEventEnabled) return;

		onMouseScrolled(mouse);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->mouseScrolled(mouse);
		}
	}

	void ofxComponentBase::dragEvent(ofDragInfo& dragInfo) {
		if (!isActive || !keyMouseEventEnabled) return;

		onDragEvent(dragInfo);
		for (int i = 0; i < children.size(); ++i) {
			auto& c = children[i];
			c->dragEvent(dragInfo);
		}
	}

	void ofxComponentBase::setKeyMouseEventEnabled(bool enabled) {
		keyMouseEventEnabled = enabled;
	}

	bool ofxComponentBase::toggleKeyMouseEventEnabled() {
		return keyMouseEventEnabled = !keyMouseEventEnabled;
	}

	bool ofxComponentBase::getKeyMouseEventEnebled() {
		return keyMouseEventEnabled;
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

	ofVec2f ofxComponentBase::getCenterPos() {
		return rect.getCenter();
	}

	ofVec2f ofxComponentBase::getGlobalPos() {
		return localToGlobalPos(ofVec2f());
	}

	ofVec2f ofxComponentBase::getGlobalCenterPos() {
		return localToGlobalPos(ofVec2f(rect.width/2, rect.height/2));
	}

	Alignment ofxComponentBase::getScaleAlignment() {
		return scaleAlignment;
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
		if (rect == _rect) return;
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

	void ofxComponentBase::setScaleAlignment(Alignment _alignment) {
		if (scaleAlignment == _alignment) return;
		scaleAlignment = _alignment;
		updateMatrix();
	}

	void ofxComponentBase::setWidth(float _width) {
		setRect(ofRectangle(rect.x, rect.y, _width, rect.height));
	}

	void ofxComponentBase::setHeight(float _height) {
		setRect(ofRectangle(rect.x, rect.y, rect.width, _height));
	}

	void ofxComponentBase::setScale(float _scale) {
		if (scale == _scale) return;
		scale = _scale;
		updateMatrix();
	}

	void ofxComponentBase::setRotation(float _rotation) {
		if (rotation == _rotation) return;
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

	bool ofxComponentBase::inside(ofVec2f p) {
		return inside(p.x, p.y);
	}

	bool ofxComponentBase::inside(float x, float y) {
		return ofRectangle(0, 0, rect.width, rect.height).inside(x, y);
	}

	bool ofxComponentBase::isMouseInside() {
		return inside(getMouseX(), getMouseY());
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

		updateGlobalMatrix();
	}

	void ofxComponentBase::removeParent() {
		if (parent != nullptr) {
			auto p = parent;
			parent = nullptr;
			p->removeChild(shared_from_this());
		}
	}

	void ofxComponentBase::addChild(shared_ptr<ofxComponentBase>  _child) {
		// insert to back
		insertChild(_child, children.size());
	}

	void ofxComponentBase::insertChild(shared_ptr<ofxComponentBase>  _child, int index) {
		bool alreadyListed = false;

		for (auto& c : children) {
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

	void ofxComponentBase::destroy() {
		if (destroyed) return;

		destroyed = true;
		destroyedComponents.push_back(shared_from_this());
		for (auto& c : children) {
			c->destroy();
		}

		clearTimerFunctions();
	}

	void ofxComponentBase::destroy(float delaySec) {
		if (destroyed) return;
		addTimerFunction([this] { destroy(); }, delaySec);
	}

	bool ofxComponentBase::isDestroyed() {
		return destroyed;
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
		if (scaleAlignment == Center) localMatrix.translate(ofVec3f(-rect.width / 2, -rect.height / 2, 0));
		localMatrix.rotateRad(rotation * DEG_TO_RAD, 0, 0, 1);
		localMatrix.scale(ofVec3f(scale, scale, scale));
		localMatrix.translate(ofVec3f(rect.x, rect.y, 0));
		if (scaleAlignment == Center) localMatrix.translate(ofVec3f(rect.width / 2, rect.height / 2, 0));

		localMatrixInverse = localMatrix.getInverse();

		onLocalMatrixChanged();
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

		for (auto& c : children) {
			c->updateGlobalMatrix();
		}
	}
	ofxComponentBase::Timer::Timer(TimerFunc func, float wait)
		: function(func) {
		execTime = wait + ofGetElapsedTimef();
	}
}