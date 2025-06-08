#pragma once

#include "ofMain.h"
#include "ofxComponent.h"
#include "ToastMessage.h"
#include "MessageChain.h"
using namespace ofxComponent;

/**
 * Timer Example App - Demonstrates ofxComponent timer features
 * 
 * This example shows:
 * 1. Toast messages with auto-hide timers and cancellation
 * 2. Message chains with pause/resume/stop control
 * 3. Proper ofxComponentManager usage and setup
 * 
 * Key concepts demonstrated:
 * - addTimerFunction() for delayed execution
 * - setTimerPaused() for pause/resume control
 * - clearTimerFunctions() for canceling all timers
 * - Timer cancellation with stored references
 */
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
    
    // Event handler methods for component interactions
    void onToastButtonClicked();
    void onStartButtonClicked();
    void onPauseButtonClicked();
    void onStopButtonClicked();
    
private:
    // Component manager - handles all events automatically after setup()
    shared_ptr<ofxComponentManager> manager;
    
    // Demo components
    shared_ptr<ToastMessage> toast;         // Shows temporary messages
    shared_ptr<MessageChain> messageChain;  // Demonstrates timer chains
    
    // UI buttons
    shared_ptr<ofxComponentBase> toastButton;
    shared_ptr<ofxComponentBase> animStartButton;
    shared_ptr<ofxComponentBase> animPauseButton;
    shared_ptr<ofxComponentBase> animStopButton;
    
    // Counter for toast messages
    int toastCounter = 0;
};
