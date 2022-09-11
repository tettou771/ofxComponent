# ofxComponent

## Overview

This is simple component system.

I made it because I was wanna layout objects with parent-children model likes Unity, and make this addon.

## Features

- You can set position and rotation and scale, without no effect in local
- Get local coordinate position easily. 
  - getMousePos() <- local position
  - globalToLocalPos(ofVec2f) <- convert global to local
  - localToGlobalPos(ofVec2f) <- convert local to global
  - getLocalMatrix(), getGlobalMatrix(), etc...
- Draggable if you need
  - setDraggable(true)

## Depent addons

- It doesn't depend other addon.

## Tested system

- of0.10.1 or later

## Usage

Define your custom comonent (for ex, MyComponent) as derived class of ofxComponentBase.

All components should be instantiated as shard_ptr<T> and should be removed using destroy().

And override some methods if it has task.

Methods detail is in ofxComponentBase.h, cpp.

### Initialize method

```cpp
// Called once after setup()
// Not called if make object after setup().
onSetup();

// Called once before first onUpdate().
onStart();
```

### Every frame

```cpp
// Called in update()
onUpdate();

// Called in draw() after parent
onDraw();

// Called in draw() after children
postDraw();
```

### Input

If you set setKeyMouseEventEnabled(false), ignore that.

In default, enabled.

```cpp
onKeyPressed(ofKeyEventArgs&);
onKeyReleased(ofKeyEventArgs&);
onMouseMoved(ofMouseEventArgs&);
onMousePressed(ofMouseEventArgs&);
onMouseDragged(ofMouseEventArgs&);
onMouseReleased(ofMouseEventArgs&);
onDragEvent(ofDragInfo&);
```

### State changed handler

```cpp
// Called after setActive(bool)
// if active is false, it is ignored in update, draw, and more
onActiveChanged(bool);

// Called after setPos(), setWidth(), setScale(), and more
onLocalMatrixChanged();

// Called after destroy() it.
onDestroy();
```

### MyComponent sample
```cpp:MyComponent.h
#include "ofxComponent.h"
using namespace ofxComponent;

class MyComponent : public ofxComponentBase {
	void onStart() override {
		// set component size (if you wanna)
		setWidth(200);
		setHeight(100);

		// set component position x, y (if you wanna)
		setPos(50, 50);
	}
	void onDraw() override {
		// draw you wanna
		ofSetColor(ofColor::yellow);
		// getWidth(), getHeight() is ofxComponentBase member
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}
};

```

### ofApp sample

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

Setup componentManager and set MyComponent as children.


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

Please see the example for detail.

## Welcome to pull request

This is incomplete code.

Please improve it if you need.