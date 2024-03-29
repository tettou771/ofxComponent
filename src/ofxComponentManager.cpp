#include "ofxComponentManager.h"


namespace ofxComponent {
    shared_ptr<ofxComponentManager> ofxComponentManager::singleton = nullptr;
    
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
        ofRemoveListener(ofEvents().mouseScrolled, this, &ofxComponentManager::mouseScrolled, OF_EVENT_ORDER_BEFORE_APP);
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
        ofAddListener(ofEvents().mouseScrolled, this, &ofxComponentManager::mouseScrolled, OF_EVENT_ORDER_BEFORE_APP);
        ofAddListener(ofEvents().fileDragEvent, this, &ofxComponentManager::dragEvent, OF_EVENT_ORDER_BEFORE_APP);
        
        singleton = static_pointer_cast<ofxComponentManager>(shared_from_this());
    }
    
    void ofxComponentManager::update(ofEventArgs &args) {
        ofxComponentBase::update(args);
        
        removeDestroyedObjects();
    }
    void ofxComponentManager::draw(ofEventArgs &args) {
        ofxComponentBase::draw(args);
    }
    void ofxComponentManager::exit(ofEventArgs &args) {
        destroy();
        removeDestroyedObjects();
        ofxComponentBase::exit(args);
    }
    void ofxComponentManager::keyPressed(ofKeyEventArgs &key) {
        ofxComponentBase::keyPressed(key);
    }
    void ofxComponentManager::keyReleased(ofKeyEventArgs &key) {
        ofxComponentBase::keyReleased(key);
    }
    void ofxComponentManager::mouseMoved(ofMouseEventArgs &mouse) {
        checkMostTopComponent();
        ofxComponentBase::mouseMoved(mouse);
    }
    void ofxComponentManager::mousePressed(ofMouseEventArgs &mouse) {
        checkMostTopComponent();
        ofxComponentBase::mousePressed(mouse);
    }
    void ofxComponentManager::mouseDragged(ofMouseEventArgs &mouse) {
        checkMostTopComponent();
        ofxComponentBase::mouseDragged(mouse);
    }
    void ofxComponentManager::mouseReleased(ofMouseEventArgs &mouse) {
        checkMostTopComponent();
        ofxComponentBase::mouseReleased(mouse);
    }
    void ofxComponentManager::mouseScrolled(ofMouseEventArgs &mouse) {
        checkMostTopComponent();
        ofxComponentBase::mouseScrolled(mouse);
    }
    void ofxComponentManager::dragEvent(ofDragInfo &dragInfo) {
        checkMostTopComponent();
        ofxComponentBase::dragEvent(dragInfo);
    }
    
    void ofxComponentManager::checkMostTopComponent() {
        mouseOverComponent = nullptr;
        std::function<void(vector<shared_ptr<ofxComponentBase> >)> checkMostTop = [&](vector<shared_ptr<ofxComponentBase> > list){
            for (shared_ptr<ofxComponentBase> c : list) {
                if (!c->getActive()) continue;
                vector<shared_ptr<ofxComponentBase> > clist = c->getChildren();
                if (c->isMouseInside()) {
                    mouseOverComponent = c;
                    if (c->getConstrain()) {
                        checkMostTop(clist);
                    }
                }
                if (!c->getConstrain()) {
                    checkMostTop(clist);
                }
            }
        };
        
        auto list = getChildren();
        checkMostTop(list);
    }
    
    void ofxComponentManager::removeDestroyedObjects() {
        // remove destroyed object
        while (!destroyedComponents.empty()) {
            auto c = destroyedComponents.front();
            if (c->isDestroyed()) {
                for (auto& cc : c->getChildren()) {
                    cc->removeParent();
                }
                c->removeParent();
                
                c->onDestroy();
                destroyedComponents.erase(destroyedComponents.begin());
                
                auto itr = find(allComponents.begin(), allComponents.end(), c);
                if (itr != allComponents.end()) {
                    allComponents.erase(itr);
                }
            }
        }
    }
}
