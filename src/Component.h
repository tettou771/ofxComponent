#pragma once
#include "ofMain.h"

namespace ofxComponent {
	class Component {
	public:
		Component();
		~Component();

		// of events
		void setup();
		virtual void onSetup() {}

		void draw(ofEventArgs &);
		virtual void onDraw() {}
		virtual void postDraw() {}

		void update(ofEventArgs &);
		virtual void onUpdate() {}
		virtual void postUpdate() {}

		void exit(ofEventArgs &);
		virtual void onExit() {}

		// acitive / disactive
		void setActive(bool);
		bool getActive() { return isActive; }
		virtual void onActiveChanged() { ; }

		// key
		void keyPressed(ofKeyEventArgs &);
		virtual void onKeyPressed(ofKeyEventArgs &) {}

		void keyReleased(ofKeyEventArgs &);
		virtual void onKeyReleased(ofKeyEventArgs &) {}

		// mouse
		void mouseMoved(ofMouseEventArgs &);
		virtual void onMouseMoved(ofMouseEventArgs &) {}

		void mousePressed(ofMouseEventArgs &);
		virtual void onMousePressed(ofMouseEventArgs &) {}

		void mouseDragged(ofMouseEventArgs &);
		virtual void onMouseDragged(ofMouseEventArgs &) {}

		void mouseReleased(ofMouseEventArgs &);
		virtual void onMouseReleased(ofMouseEventArgs &) {}

		ofRectangle getRect();
		ofRectangle getRectGlobal();
		ofVec2f getPos();
		float getParentWidth();
		float getParentHeight();
		float getWidth();
		float getHeight();
		float getScale();
		float getRotation();
		void setRect(ofRectangle);
		void setPos(float, float);
		void setPos(ofVec2f _pos);
		void setWidth(float);
		void setHeight(float);
		void setScale(float);
		void setRotation(float);

		// convert coordinated position
		ofVec2f globalToLocalPos(ofVec2f _globalPos);
		ofVec2f localToGlobalPos(ofVec2f _componentPos);
		float getMouseX();
		float getMouseY();
		ofVec2f getMousePos();
		ofVec2f getPreviousMousePos();
		ofMatrix4x4 getLocalMatrix();
		ofMatrix4x4 getLocalMatrixInverse();
		ofMatrix4x4 getGlobalMatrix();
		ofMatrix4x4 getGlobalMatrixInverse();

		// drag
		void setDraggable(bool _draggable);
		bool getDraggable();
		bool getDragging();

		void setParent(Component *_parent);
		void removeParent();
		void addChild(Component *_child);
		void insertChild(Component *_child, int index);
		void removeChild(Component *_child);

	private:
		bool isActive = true;
		ofRectangle rect;
		float scale = 1.0;
		float rotation = 0;
		ofMatrix4x4 localMatrix, localMatrixInverse, globalMatrix, globalMatrixInverse;
		void updateMatrix();
		void updateGlobalMatrix();

		bool draggable = false, dragging = false;

		Component * parent = nullptr;
		vector<Component *> children;
	};
}