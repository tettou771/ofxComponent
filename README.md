# ofxComponent

This is an addon for openFrameworks (oF) called ofxComponent.
A convenient tool for creating UIs within oF was lacking, so this addon was created.

The purpose of this tool is to simplify the organization of object hierarchies (parent-child relationships) in UI components, such as using relative mouse coordinates within each object and organizing object position relationships within a relative coordinate system.

The drawing order is also considered, ensuring that parent objects are drawn before child objects, making it easier to create structures such as UI panels with buttons and views. Specifically, you can set relative positions using the setPos() method, obtain your own coordinates relative to the parent using getPos(), and obtain relative mouse coordinates within your coordinate system using getMousePos().

# Depend addons

- It doesn't depend on any other addons.

# Tested system

- of0.10.1 or later

# Usage

To get started, please refer to the examples provided.
To use this tool, you need to inherit all objects you create from the ofxComponentBase class and manage them using shared_ptr. Please use shared_ptr with caution, paying attention to its features. Establish parent-child relationships using the setChild() method to link parent and child objects. Parent-child relationships can change during runtime.

There is also an ofxComponentManager class to manage the entire object hierarchy. You need to create only one instance of this class and set it as the top-level parent:

```cpp
// ofApp.h ofApp
shared_ptr<ofxComponentManager> manager;
```
Define one instance like this, and then:

```cpp
// ofApp.cpp ofApp::setup()

manager->setChild(yourComponent);
// ...and more children...

manager->setup();
```

When removing an object, call destroy() and it will be removed during the next update. Since it is not removed immediately, you may need to use isDestroyed() to check whether the object has been removed as needed.

The ofxComponentBase class (header file provided) contains various methods and attributes to manage and control components, such as positioning, scaling, rotation, mouse and key events, and drawing constraints. It also includes methods for managing parent-child relationships, converting coordinate positions, and handling component destruction.

For more information on the usage and implementation details of ofxComponent, refer to the ofxComponentBase.h header file provided.

## onUpdate, onDraw as Unity

When you override these methods, you can handle events.

- `onStart();` // is Called once before the first onUpdate() after the object is created.
- `onUpdate();`
- `onDraw();`


## Input handler

- `onKeyPressed(ofKeyEventArgs&);`
- `onKeyReleased(ofKeyEventArgs&);`
- `onMouseMoved(ofMouseEventArgs&);`
- `onMousePressed(ofMouseEventArgs&);`
- `onMouseDragged(ofMouseEventArgs&);`
- `onMouseReleased(ofMouseEventArgs&);`
- `onDragEvent(ofDragInfo&);`

### Mouse Event Details

#### onMousePressedOverComponent vs onMousePressed

- **`onMousePressed(ofMouseEventArgs&)`**: Called when mouse is pressed anywhere on the component, even if other components are layered on top
- **`onMousePressedOverComponent(ofMouseEventArgs&)`**: Called only when the component is the topmost (visible) component at the click position

Use `onMousePressedOverComponent` for button-like interactions where you want clicks to only register on the visible top layer.

#### isMouseOver() Method

- **`isMouseOver()`**: Returns true if the mouse cursor is over this component AND this component is the topmost at that position
- Useful for implementing hover states in UI elements
- Automatically handles layer checking - no need to manually verify if other components are on top

#### Mouse Event System Integration

Components provide built-in `ofEvent` instances for standard event handling:

```cpp
// Built-in events you can listen to:
ofEvent<void> mousePressedOverComponentEvents;  // Triggered only for topmost component
ofEvent<ofMouseEventArgs> mousePressedEvents;   // Triggered for any component under cursor
// ... and more for other mouse events
```

Example usage:
```cpp
// In your ofApp setup:
ofAddListener(myButton->mousePressedOverComponentEvents, this, &ofApp::onButtonClicked);

// Event handler method:
void ofApp::onButtonClicked() {
    // Handle button click
}
```

## Timer System

ofxComponent provides a built-in timer system for scheduling delayed function execution. This is particularly useful for animations, UI transitions, auto-hide functionality, and other time-based behaviors.

### Basic Timer Usage

#### addTimerFunction()

Schedule a function to execute after a specified delay:

```cpp
// Basic usage - execute after 2 seconds
auto timer = addTimerFunction([]() {
    ofLog() << "Timer executed!";
}, 2.0f);

// With component context
auto timer = addTimerFunction([this]() {
    this->hide();  // Hide this component after delay
}, 1.5f);
```

The timer returns a `shared_ptr<Timer>` that you can use to control the timer later.

### Timer Control Methods

#### setTimerPaused() - Component-Level Control

Pause or resume ALL timers associated with a component:

```cpp
setTimerPaused(true);   // Pause all timers in this component
setTimerPaused(false);  // Resume all timers in this component

// Check if timers are paused
if (isTimerPaused()) {
    // Component timers are paused
}
```

This automatically applies to:
- All existing timers in the component
- All child components (hierarchical pausing)
- New timers created while paused will inherit the paused state

#### clearTimerFunctions() - Cancel All Timers

Cancel all pending timers in a component:

```cpp
clearTimerFunctions();  // Safely cancel all timers
```

This is the recommended way to stop all timer activity, especially when components are being destroyed or reset.

### Individual Timer Control

You can control individual timers using the returned timer reference:

```cpp
// Store timer reference
auto myTimer = addTimerFunction([this]() {
    // Timer function
}, 3.0f);

// Later, control the individual timer
myTimer->cancel();           // Cancel this specific timer
myTimer->setPaused(true);    // Pause this specific timer
myTimer->setPaused(false);   // Resume this specific timer

// Check timer state
if (myTimer->isPaused()) {
    // Timer is paused
}
if (myTimer->isDone()) {
    // Timer has been executed or cancelled
}
```

### Timer System Features

- **Time units**: All timer functions use seconds (not milliseconds)
- **Automatic cleanup**: Executed and cancelled timers are automatically removed
- **Pause preservation**: When paused timers resume, they maintain their remaining time
- **Hierarchical control**: Parent component pause state affects all children
- **Memory safe**: Uses `shared_ptr` and `weak_ptr` to prevent memory leaks
- **Thread safe**: Designed for single-threaded openFrameworks usage

### Common Patterns

#### Auto-hide Toast Messages
```cpp
void showToast(const string& message, float duration = 2.0f) {
    isVisible = true;
    displayText = message;
    
    // Cancel any existing hide timer
    if (hideTimer) hideTimer->cancel();
    
    // Schedule auto-hide
    hideTimer = addTimerFunction([this]() {
        isVisible = false;
    }, duration);
}
```

#### Sequential Animation Chain
```cpp
void startAnimationSequence() {
    // Step 1: Fade in
    addTimerFunction([this]() {
        startFadeIn();
    }, 0.0f);
    
    // Step 2: Wait, then move
    addTimerFunction([this]() {
        startMovement();
    }, 1.0f);
    
    // Step 3: Final action
    addTimerFunction([this]() {
        onAnimationComplete();
    }, 3.0f);
}
```

#### Pause/Resume Control
```cpp
void pauseAllActivity() {
    setTimerPaused(true);  // Pauses ALL timers in this component and children
}

void resumeAllActivity() {
    setTimerPaused(false); // Resumes ALL timers, maintaining remaining time
}

void stopAllActivity() {
    clearTimerFunctions();  // Cancels ALL timers permanently
}
```

### Example Implementation

See `exampleTimer` for a complete working example that demonstrates:
- Toast messages with auto-hide timers
- Sequential message chains with pause/resume/stop controls
- Timer cancellation and management
- Event-driven timer control using component events

## Position, Herarchy, etc

- `setPos(ofVec2f pos);` // To set the relative position of an object
- `getPos();` // To get an object's position relative to its parent
- `getMousePos();` // To get the mouse position relative to the object's coordinate system
- `setChild(shared_ptr<ofxComponentBase> child);` // To establish a parent-child relationship between objects
- `getChildren();` // To get a list of child objects
- `setActive(bool active);` // To set an object as active or inactive

# Examples

This addon includes several examples to help you get started:

## Basic Example (`example/`)

A simple demonstration of basic component usage and hierarchy.

## Timer Example (`exampleTimer/`)

A comprehensive demonstration of the timer system featuring:
- Toast messages with auto-hide functionality
- Sequential message chains with pause/resume/stop controls
- Event-driven interactions using `mousePressedOverComponentEvents`
- Proper timer management and cancellation

This example is particularly useful for learning time-based UI interactions and component event handling.

# Sample Implementation

## MyComponent sample

```cpp:MyComponent.h
#include "ofxComponent.h"
using namespace ofxComponent;

class MyComponent : public ofxComponentBase {
	void onStart() override {
		// set component size
		setWidth(200);
		setHeight(100);

		// set relative position
		setPos(50, 50);
	}
	void onDraw() override {
		// draw background
		ofSetColor(ofColor::yellow);
		// getWidth(), getHeight() returns component size.
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}
	void onMousePressedOverComponent(ofMouseEventArgs& mouse) override {
		ofLog() << "Clicked";
	}
};
```

## ofApp Integration

### Header file setup

Include the header and use the namespace:

```cpp:ofApp.h
#include "ofxComponent.h"
using namespace ofxComponent;

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    // ... other standard ofApp methods ...

private:
    // IMPORTANT: Create manager as shared_ptr, NOT using getInstance()
    shared_ptr<ofxComponentManager> manager;
    
    // Your components
    shared_ptr<MyComponent> myComp;
};
```

### Implementation setup

**CRITICAL**: Follow this exact setup pattern for proper component system initialization:

```cpp:ofApp.cpp
void ofApp::setup(){
    // 1. Create the manager as shared_ptr
    manager = make_shared<ofxComponentManager>();
    
    // 2. Set the manager's drawing area (typically full window)
    manager->setRect(0, 0, ofGetWidth(), ofGetHeight());
    
    // 3. Create your components
    myComp = make_shared<MyComponent>();
    
    // 4. Add components to manager (NOT to a separate root component)
    manager->addChild(myComp);
    
    // 5. IMPORTANT: Call setup() AFTER adding all components
    //    This registers the manager's event listeners with openFrameworks
    manager->setup();
}

// IMPORTANT: Leave these methods empty!
// The manager automatically handles all events after setup() is called
void ofApp::update() {
    // Empty - manager handles update automatically
}

void ofApp::draw() {
    // Empty - manager handles drawing automatically  
}

void ofApp::mousePressed(int x, int y, int button) {
    // Empty - manager handles mouse events automatically
}

// ... all other ofApp event methods can be left empty
```

### Key Points for ofxComponentManager

1. **Use shared_ptr**: Always create as `make_shared<ofxComponentManager>()`, never use `getInstance()`
2. **Set drawing area**: Call `setRect()` to define the manager's coordinate space
3. **Add children directly**: Add components to the manager, not to a separate root component
4. **Call setup() last**: This registers automatic event listeners with openFrameworks
5. **Empty ofApp methods**: After `manager->setup()`, you don't need to forward events manually

### What NOT to do

```cpp
// DON'T: Use getInstance() pattern
auto manager = ofxComponentManager::getInstance(); // This doesn't exist

// DON'T: Manually forward events in ofApp methods
void ofApp::update() {
    manager->update(); // Unnecessary - manager handles this automatically
}

// DON'T: Create separate root components
auto root = make_shared<ofxComponentBase>();
manager->addChild(root);
root->addChild(myComp); // Instead, add directly to manager
```

This is in development, so there may be major changes from time to time.

# Author

http://github.com/tettou771

# License

MIT