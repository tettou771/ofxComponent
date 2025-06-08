#pragma once
#include "ofxComponent.h"
using namespace ofxComponent;

/**
 * ToastMessage - A simple toast notification component demonstrating timer cancellation
 * 
 * This class demonstrates:
 * - addTimerFunction() for auto-hiding after a delay
 * - Timer cancellation when showing new messages
 * - Storing timer references to control them later
 * 
 * The component shows temporary messages that automatically disappear after a set duration.
 * If a new message is shown before the previous one expires, the old timer is cancelled.
 */
class ToastMessage : public ofxComponentBase {
public:
    ToastMessage() {
        name = "ToastMessage";
        setWidth(300);
        setHeight(50);
    }
    
    /**
     * Show a toast message for the specified duration
     * 
     * @param message The text to display
     * @param durationSec How long to show the message (in seconds)
     * 
     * This demonstrates timer cancellation - if a timer is already running,
     * it gets cancelled before creating a new one. This prevents multiple
     * overlapping timers and ensures predictable behavior.
     */
    void show(const string& message, int durationSec = 2) {
        messageText = message;
        isVisible = true;
        
        // Cancel any existing timer before creating a new one
        // This demonstrates how to properly manage timer references
        if (hideTimer) {
            hideTimer->cancel();
        }
        
        // Create a new timer to hide the message after the specified duration
        hideTimer = addTimerFunction([this]() {
            isVisible = false;
        }, durationSec);
    }
    
protected:
    /**
     * Draw the toast message if it's currently visible
     * Only draws when isVisible is true - the timer automatically sets this to false
     */
    void onDraw() override {
        if (!isVisible) return;
        
        // Draw semi-transparent background
        ofSetColor(50, 50, 50, 200);
        ofDrawRectRounded(0, 0, getWidth(), getHeight(), 5);
        
        // Draw the message text
        ofSetColor(255);
        ofDrawBitmapString(messageText, 10, getHeight() / 2 + 5);
    }
    
private:
    // Private member variables
    string name;                     // Component name for debugging
    string messageText;              // The current message text to display
    bool isVisible = false;          // Whether the toast is currently visible
    shared_ptr<Timer> hideTimer;     // Timer reference for auto-hiding (can be cancelled)
};
