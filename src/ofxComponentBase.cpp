#include "ofxComponentBase.h"

using namespace ofxComponent;
vector<shared_ptr<ofxComponentBase> > ofxComponentBase::allComponents;
vector<shared_ptr<ofxComponentBase> > ofxComponentBase::destroyedComponents;
shared_ptr<ofxComponentBase> ofxComponentBase::movingComponent = nullptr;
shared_ptr<ofxComponentBase>  ofxComponentBase::mouseOverComponent = nullptr;

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
        for (auto i = 0; i < timerFunctions.size(); ++i) {
            {
                auto tf = timerFunctions[i];
                if (tf->canceled) {
                    delete tf;
                    timerFunctions.erase(timerFunctions.begin() + i);
                    i--;
                }
                else if (tf->execTime <= now) {
                    tf->function();
                    delete tf;
                    timerFunctions.erase(timerFunctions.begin() + i);
                    i--;
                }
                else {
                }
            }
        }
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
    
    // draw in constrainFbo
    bool isCurrentConstrain = constrain;
    if (isCurrentConstrain) {
        // resize fbo if needed
        if (getWidth() > 0 && getHeight() > 0 &&
            (getWidth() != constrainFbo.getWidth() || getHeight() != constrainFbo.getHeight())) {
            constrainFbo.allocate(getWidth(), getHeight());
        }
        
        if (!constrainFbo.isAllocated()) {
            constrainFbo.allocate(MAX(1, getWidth()), MAX(1, getHeight()));
        }
        
        // draw in fbo
        constrainFbo.begin();
        ofClear(0, 0, 0, 0);
    }
    
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
    
    if (isCurrentConstrain) {
        constrainFbo.end();
        constrainFbo.draw(0, 0);
    }
    
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
    if (!destroyed) {
        allComponents.push_back(shared_from_this());
    }
    
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
    timerFunctions.push_back(new Timer(func, wait));
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
        return false;
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
    if (constrain && !isMouseInside()) return;
    
    onMouseMoved(mouse);
    for (int i = 0; i < children.size(); ++i) {
        auto& c = children[i];
        c->mouseMoved(mouse);
    }
}

void ofxComponentBase::mousePressed(ofMouseEventArgs& mouse) {
    if (!isActive || !keyMouseEventEnabled) return;
    if (constrain && !isMouseInside()) return;
    
    if (movable && isMouseOver()) {
        setMoving(true);
    }
    
    onMousePressed(mouse);
    
    // if this object is top (on mouse)
    if (isMouseOver()) {
        mousePressedOverComponent = true;
        onMousePressedOverComponent(mouse);
        ofNotifyEvent(mousePressedOverComponentEvents);
    }
    
    for (int i = 0; i < children.size(); ++i) {
        auto& c = children[i];
        c->mousePressed(mouse);
    }
}

void ofxComponentBase::mouseDragged(ofMouseEventArgs& mouse) {
    if (!isActive || !keyMouseEventEnabled) return;
    if (constrain && !isMouseInside()) return;
    
    if (getMoving()) {
        ofVec2f move = getMousePos() - getPreviousMousePos();
        setPos(getPos() + move);
    }
    
    onMouseDragged(mouse);
    for (auto& c : children) {
        c->mouseDragged(mouse);
    }
}

void ofxComponentBase::mouseReleased(ofMouseEventArgs& mouse) {
    mousePressedOverComponent = false;

    if (!isActive || !keyMouseEventEnabled) return;
    if (constrain && !isMouseInside()) return;
    
    if (getMoving()) setMoving(false);
    
    onMouseReleased(mouse);
    for (int i = 0; i < children.size(); ++i) {
        auto& c = children[i];
        c->mouseReleased(mouse);
    }
}

void ofxComponentBase::mouseScrolled(ofMouseEventArgs& mouse) {
    if (!isActive || !keyMouseEventEnabled) return;
    if (constrain && !isMouseInside()) return;
    
    onMouseScrolled(mouse);
    for (int i = 0; i < children.size(); ++i) {
        auto& c = children[i];
        c->mouseScrolled(mouse);
    }
}

void ofxComponentBase::dragEvent(ofDragInfo& dragInfo) {
    if (!isActive || !keyMouseEventEnabled) return;
    if (constrain && !isMouseInside()) return;
    
    onDragEvent(dragInfo);
    for (int i = 0; i < children.size(); ++i) {
        auto& c = children[i];
        c->dragEvent(dragInfo);
    }
}

bool ofxComponentBase::isMouseInside() {
    return inside(getMouseX(), getMouseY());
}

bool ofxComponentBase::isMouseOver() {
    return mouseOverComponent == shared_from_this();
}

bool ofxComponentBase::isMousePressedOverComponent() {
    return mousePressedOverComponent;
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
    return localToGlobalPos(ofVec2f(rect.width / 2, rect.height / 2));
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
    if (isnan(_rect.x) || isnan(_rect.y) || isnan(_rect.width) || isnan(_rect.height)) return;
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
    if (isnan(_width)) return;
    setRect(ofRectangle(rect.x, rect.y, _width, rect.height));
}

void ofxComponentBase::setHeight(float _height) {
    if (isnan(_height)) return;
    setRect(ofRectangle(rect.x, rect.y, rect.width, _height));
}

void ofxComponentBase::setScale(float _scale) {
    if (isnan(_scale)) return;
    if (scale == _scale) return;
    scale = _scale;
    updateMatrix();
}

void ofxComponentBase::setRotation(float _rotation) {
    if (isnan(_rotation)) return;
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

void ofxComponentBase::setMovable(bool _movable) {
    movable = _movable;
    if (!movable) setMoving(false);
}

bool ofxComponentBase::getMovable() {
    return movable;
}

void ofxComponentBase::setMoving(bool _moving) {
    if (_moving) movingComponent = shared_from_this();
    else if (getMoving()) movingComponent = nullptr;
}

bool ofxComponentBase::getMoving() {
    return !needStartExec && movingComponent == shared_from_this();
}

bool ofxComponentBase::inside(ofVec2f p) {
    return inside(p.x, p.y);
}

bool ofxComponentBase::inside(float x, float y) {
    return ofRectangle(0, 0, rect.width, rect.height).inside(x, y);
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
    int alreadyListedIndex = -1;
    
    int i = 0;
    for (auto& c : children) {
        if (c == _child) {
            children.erase(children.begin() + i);
            alreadyListedIndex = i;
            break;
        }
        ++i;
    }
    
    int offsetIndex = index;
    if (alreadyListedIndex < index) offsetIndex -= 1;
    
    // add to back if index is over
    if (children.size() <= offsetIndex) {
        children.push_back(_child);
    }
    
    else if (0 <= offsetIndex) {
        children.insert(children.begin() + offsetIndex, _child);
    }
    
    else {
        children.insert(children.begin(), _child);
    }
    
    if (alreadyListedIndex == -1) {
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

void ofxComponentBase::swapChild(int indexA, int indexB) {
    if (indexA < 0 || indexB < 0 || indexA >= children.size() || indexB >= children.size()) {
        ofLogWarning() << "swapChild(): invalid index " << indexA << " and " << indexB;
        return;
    }
    if (indexA == indexB) {
        return;
    }
    auto A = children[indexA];
    children[indexA] = children[indexB];
    children[indexB] = A;
}

void ofxComponentBase::setConstrain(bool _constrain) {
    constrain = _constrain;
}

bool ofxComponentBase::getConstrain() {
    return constrain;
}

void ofxComponentBase::destroy() {
    if (destroyed) return;
    
    destroyed = true;
    setMoving(false);
    
    if (!needStartExec) {
        destroyedComponents.push_back(shared_from_this());
    }

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
    ofNotifyEvent(localMatrixChangedEvents);
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

