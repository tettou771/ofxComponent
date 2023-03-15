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

## Position, Herarchy, etc

- `setPos(ofVec2f pos);` // To set the relative position of an object
- `getPos();` // To get an object's position relative to its parent
- `getMousePos();` // To get the mouse position relative to the object's coordinate system
- `setChild(shared_ptr<ofxComponentBase> child);` // To establish a parent-child relationship between objects
- `getChildren();` // To get a list of child objects
- `setActive(bool active);` // To set an object as active or inactive

# Sample
MyComponent sample

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
	void onMousePressedOverComponent(ofMouseEventArgs& mouse) {
		ofLog() << "Clicked";
	}
};

```

ofApp

Include this and use namespace "ofxComponent".

Instance of ofxComponentManager is need to run component system.

```cpp:ofApp.h
#include "ofxComponent.h"
using namespace ofxComponent;

class ofApp : public ofBaseApp{

    ... some definissions ...

    // ofxComponent needs this instance
    shared_ptr<ofxComponent::ofxComponentManager> manager;
};
```

Set up the componentManager and add MyComponent as a child.


```cpp:ofApp.cpp
void ofApp::setup(){
	// Set drawing area
	manager = make_shared<ofxComponentManager>();

	myComp = make_shared<MyComponent>();

	// Add children
	manager->addChild(myComp);

	// Setup all objects
	// Please call this after children add
	manager->setup();
}

```

This is in development, so there may be major changes from time to time.

# Author

http://github.com/tettou771

# Lisence

MIT