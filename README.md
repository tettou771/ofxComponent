# ofxComponent

## Overview

This is simple component system.

ofxComponent can that.

- Layout with parent-children model
- You can set position and rotation and scale, without no effect in local
- Get local coordinate position easily. 
  - getMousePos() <- local position
  - globalToLocalPos(ofVec2f) <- convert global to local
  - localToGlobalPos(ofVec2f) <- convert local to global
  - getLocalMatrix(), getGlobalMatrix(), etc...
- Draggable if you need
  - setDraggable(true)

## Requirement

- It doesn't depend other addon.

## Tested system

- Windows 10 + VS2017 + of0.10.1

## Usage

Please see the example.

## Welcome to pull request

This is incomplete code.

Please improve it if you need.