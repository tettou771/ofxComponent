#pragma once
#include "ofxComponent.h"
using namespace ofxComponent;

/**
 * MessageChain - A simple demonstration of ofxComponent timer features
 * 
 * This class demonstrates:
 * - addTimerFunction() for scheduling delayed function calls
 * - setTimerPaused() for pausing/resuming all component timers
 * - clearTimerFunctions() for canceling all pending timers
 * 
 * The component displays a sequence of messages with 1-second delays,
 * and allows pause/resume/stop control through the timer system.
 */
class MessageChain : public ofxComponentBase {
public:
    MessageChain() {
        name = "MessageChain";
        setWidth(400);
        setHeight(100);
        // Define the message sequence to be displayed
        messages = {"Step 1: Starting", "Step 2: Processing", "Step 3: Loading", "Step 4: Finishing", "Step 5: Complete!"};
    }
    
    /**
     * Start the message chain sequence
     * Resets the progress and begins scheduling messages with 1-second delays
     */
    void startChain() {
        currentIndex = 0;
        currentMessage = "";
        scheduleNextMessage();
    }
    
    /**
     * Pause all pending timers in this component
     * Uses setTimerPaused() which automatically pauses all component timers
     */
    void pauseChain() {
        setTimerPaused(true);
    }
    
    /**
     * Resume all paused timers in this component
     * Timers will continue from where they were paused
     */
    void resumeChain() {
        setTimerPaused(false);
    }
    
    /**
     * Stop the message chain and cancel all pending timers
     * Uses clearTimerFunctions() to safely cancel all timers
     */
    void stopChain() {
        // Cancel all timers of this component - this is the safe way to stop all timers
        clearTimerFunctions();
        currentIndex = 0;
        currentMessage = "";
    }
    
protected:
    /**
     * Draw the component - shows current message, progress, and timer status
     * Background color indicates the current state: Gray=idle, Green=running, Yellow=paused
     */
    void onDraw() override {
        // Draw background with color indicating current state
        if (isTimerPaused()) {
            ofSetColor(100, 100, 50); // Yellow background when paused
        } else if (!getTimerFunctions().empty()) {
            ofSetColor(50, 100, 50); // Green background when active
        } else {
            ofSetColor(50, 50, 50); // Gray background when idle
        }
        ofDrawRectRounded(0, 0, getWidth(), getHeight(), 5);
        
        // Line 1: Display current message and overall status
        ofSetColor(255);
        string line1 = "Current: " + (currentMessage.empty() ? "None" : currentMessage);
        line1 += " | Status: ";
        if (isTimerPaused()) {
            line1 += "PAUSED";
        } else if (!getTimerFunctions().empty()) {
            line1 += "RUNNING";
        } else {
            line1 += "IDLE";
        }
        ofDrawBitmapString(line1, 10, 25);
        
        // Line 2: Show progress through the message sequence
        string line2 = "Progress: " + ofToString(currentIndex) + "/" + ofToString(messages.size());
        if (currentIndex < messages.size()) {
            line2 += " | Next: " + messages[currentIndex];
        }
        ofDrawBitmapString(line2, 10, 45);
        
        // Line 3: Debug info showing timer system internals
        auto allTimers = getTimerFunctions();
        string line3 = "Timers: " + ofToString(allTimers.size());
        if (!allTimers.empty()) {
            auto timer = allTimers[0];
            if (timer) {
                line3 += " | Paused: " + string(timer->isPaused() ? "YES" : "NO");
                line3 += " | Time: " + ofToString(timer->getExecTime(), 1) + "/" + ofToString(ofGetElapsedTimef(), 1);
            }
        }
        ofDrawBitmapString(line3, 10, 65);
    }
    
private:
    /**
     * Schedule the next message in the sequence
     * This method demonstrates recursive timer scheduling - each timer schedules the next one
     * 
     * Key timer concepts shown:
     * - addTimerFunction() creates a timer that executes after a delay
     * - The lambda [this] captures the component instance safely
     * - Recursive calling creates a chain of timers
     * - Timer respects pause/resume state automatically
     */
    void scheduleNextMessage() {
        if (currentIndex < messages.size()) {
            // Create a timer that will execute after 1 second
            addTimerFunction([this]() {
                // This lambda will be called when the timer expires
                currentMessage = messages[currentIndex];
                currentIndex++;
                
                // Recursively schedule the next message (if any remain)
                scheduleNextMessage();
            }, 1.0f); // 1 second delay (timer system works in seconds, not milliseconds)
        }
    }
    
    // Private member variables
    vector<string> messages;     // The sequence of messages to display
    int currentIndex = 0;        // Current position in the message sequence
    string currentMessage;       // Currently displayed message
    string name;                 // Component name for debugging
};
