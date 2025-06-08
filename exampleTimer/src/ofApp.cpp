#include "ofApp.h"

/**
 * SimpleButton - A basic button component for the timer demo
 * 
 * This demonstrates:
 * - mousePressedOverComponentEvents for standard event-driven click detection
 * - isMouseOver() for hover states
 * - ofEvent system integration with ofxComponent
 */
class SimpleButton : public ofxComponentBase {
public:
    SimpleButton(const string& label) : label(label) {
        setWidth(120);
        setHeight(40);
    }
    
protected:
    void onDraw() override {
        // Use isMouseOver() to show hover state
        if (isMouseOver()) {
            ofSetColor(100, 100, 100); // Lighter when hovered
        } else {
            ofSetColor(80, 80, 80);    // Normal color
        }
        ofDrawRectRounded(0, 0, getWidth(), getHeight(), 5);
        
        // Center the text on the button
        ofSetColor(255);
        float textWidth = label.length() * 8;
        float textX = (getWidth() - textWidth) / 2;
        ofDrawBitmapString(label, textX, getHeight() / 2 + 5);
    }
    
private:
    string label;
};

void ofApp::setup(){
    ofSetWindowTitle("ofxComponent Timer Example");
    ofBackground(30);
    
    // ===== IMPORTANT: How to use ofxComponentManager correctly =====
    // 
    // 1. Create the manager as a shared_ptr (NOT using getInstance())
    manager = make_shared<ofxComponentManager>();
    
    // 2. Set the manager's size to cover the full window area
    manager->setRect(0, 0, ofGetWidth(), ofGetHeight());
    
    // Helper class for displaying text labels
    class Label : public ofxComponentBase {
    public:
        Label(const string& text, float x, float y, float w, float h) : text(text) {
            setRect(x, y, w, h);
        }
    protected:
        void onDraw() override {
            ofSetColor(255);
            ofDrawBitmapString(text, 0, 20);
        }
    private:
        string text;
    };
    
    // 3. Add components to the manager (not to a separate root component)
    auto label1 = make_shared<Label>("1. Toast Message Example", 20, 20, 300, 30);
    manager->addChild(label1);
    
    // Create toast button and set up event listener
    toastButton = make_shared<SimpleButton>("Show Toast");
    toastButton->setPos(20, 60);
    // Use ofAddListener to connect to the mousePressedOverComponentEvents
    ofAddListener(toastButton->mousePressedOverComponentEvents, this, &ofApp::onToastButtonClicked);
    manager->addChild(toastButton);
    
    toast = make_shared<ToastMessage>();
    toast->setPos(20, 110);
    manager->addChild(toast);
    
    auto label2 = make_shared<Label>("2. Message Chain Example (with Timer control)", 20, 200, 300, 30);
    manager->addChild(label2);
    
    // Create animation control buttons with event listeners
    animStartButton = make_shared<SimpleButton>("Start");
    animStartButton->setPos(20, 240);
    ofAddListener(animStartButton->mousePressedOverComponentEvents, this, &ofApp::onStartButtonClicked);
    manager->addChild(animStartButton);
    
    animPauseButton = make_shared<SimpleButton>("Pause/Resume");
    animPauseButton->setPos(150, 240);
    ofAddListener(animPauseButton->mousePressedOverComponentEvents, this, &ofApp::onPauseButtonClicked);
    manager->addChild(animPauseButton);
    
    animStopButton = make_shared<SimpleButton>("Stop (Cancel)");
    animStopButton->setPos(280, 240);
    ofAddListener(animStopButton->mousePressedOverComponentEvents, this, &ofApp::onStopButtonClicked);
    manager->addChild(animStopButton);
    
    messageChain = make_shared<MessageChain>();
    messageChain->setPos(20, 300);
    manager->addChild(messageChain);
    
    class HelpText : public ofxComponentBase {
    protected:
        void onDraw() override {
            ofSetColor(200);
            string help = "Timer Features Demo:\n\n";
            help += "1. Toast Message: Click button to show temporary messages\n";
            help += "   - Uses addTimerFunction() to auto-hide after 2 seconds\n";
            help += "   - Previous timer is cancelled if new message is shown\n\n";
            help += "2. Message Chain: Sequential message display with control\n";
            help += "   - Start: Begin chain with 1-second delays between messages\n";
            help += "   - Pause/Resume: Uses setTimerPaused() to pause/resume chain\n";
            help += "   - Stop: Uses cancel() to immediately stop all pending timers\n";
            help += "   - Green = running, Yellow = paused, Gray = idle";
            ofDrawBitmapString(help, 0, 15);
        }
    };
    
    auto helpText = make_shared<HelpText>();
    helpText->setRect(20, 400, 600, 200);
    manager->addChild(helpText);
    
    // 4. IMPORTANT: Call setup() AFTER adding all components
    //    This registers the manager's event listeners with openFrameworks
    manager->setup();
    
    // ===== IMPORTANT: What you DON'T need to do =====
    // 
    // - Don't call manager->update(), manager->draw(), etc. from ofApp methods
    // - The manager automatically registers event listeners and handles everything
    // - Don't use ofxComponentManager::getInstance() - that pattern is not supported
    // - Don't manually forward events from ofApp to the manager
}

// ===== IMPORTANT: ofApp event methods =====
// 
// These methods can be left empty! The ofxComponentManager automatically
// registers its own event listeners with openFrameworks when setup() is called.
// The manager handles all events (update, draw, mouse, keyboard) automatically.

void ofApp::update(){
    // Empty - manager handles update automatically
}

void ofApp::draw(){
    // Empty - manager handles drawing automatically
}

void ofApp::keyPressed(int key){
    // Empty - manager handles keyboard input automatically
}

void ofApp::keyReleased(int key){
    // Empty - manager handles keyboard input automatically
}

void ofApp::mouseMoved(int x, int y ){
    // Empty - manager handles mouse events automatically
}

void ofApp::mouseDragged(int x, int y, int button){
    // Empty - manager handles mouse events automatically
}

void ofApp::mousePressed(int x, int y, int button){
    // Empty - manager handles mouse events automatically
}

void ofApp::mouseReleased(int x, int y, int button){
    // Empty - manager handles mouse events automatically
}

void ofApp::mouseEntered(int x, int y){
    // Empty - manager handles mouse events automatically
}

void ofApp::mouseExited(int x, int y){
    // Empty - manager handles mouse events automatically
}

void ofApp::windowResized(int w, int h){
    // Empty - manager handles window resize automatically
}

void ofApp::gotMessage(ofMessage msg){
    // Empty - not used by component system
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    // Empty - manager handles drag events automatically
}

// ===== Event handler methods for button clicks =====
// 
// These methods demonstrate the proper way to handle component events
// using the ofEvent system instead of custom callbacks

void ofApp::onToastButtonClicked(){
    toastCounter++;
    toast->show("Message #" + ofToString(toastCounter) + " - This will disappear in 2 seconds");
}

void ofApp::onStartButtonClicked(){
    messageChain->startChain();
}

void ofApp::onPauseButtonClicked(){
    if (messageChain->isTimerPaused()) {
        messageChain->resumeChain();
    } else {
        messageChain->pauseChain();
    }
}

void ofApp::onStopButtonClicked(){
    messageChain->stopChain();
}
