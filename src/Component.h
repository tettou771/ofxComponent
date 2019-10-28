#pragma once
#include "ofMain.h"

namespace ofxComponent {
	class Component : public enable_shared_from_this<Component> {
	public:
		Component();
		virtual ~Component();

		// of events
		void setup();
		virtual void onSetup() {}

		void update(ofEventArgs &);
		virtual void onUpdate() {}
		virtual void postUpdate() {}

		void draw(ofEventArgs &);
		virtual void onDraw() {}
		virtual void postDraw() {}

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

		void dragEvent(ofDragInfo &);
		virtual void onDragEvent(ofDragInfo &) {}

		ofRectangle getRect();
		ofRectangle getGlobalRect();
		ofVec2f getPos();
		ofVec2f getGlobalPos();
		float getParentWidth();
		float getParentHeight();
		float getWidth();
		float getHeight();
		float getScale();
		float getGlobalScale();
		float getRotation();
		float getGlobalRotation();
		void setRect(ofRectangle);
		void setPos(float, float);
		void setPos(ofVec2f);
		void setGlobalPos(float, float);
		void setGlobalPos(ofVec2f);
		void setCenterPos(float, float);
		void setCenterPos(ofVec2f);
		void setWidth(float);
		void setHeight(float);
		void setScale(float);
		// setGlobalScale(float); // todo
		void setRotation(float);
		// setGlobalRotation(float); // todo

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

		void setParent(shared_ptr<Component> _parent);
		void removeParent();
		void addChild(shared_ptr<Component> _child);
		void insertChild(shared_ptr<Component> _child, int index);
		void removeChild(shared_ptr<Component> _child);

		// component getter
		shared_ptr<Component> getParent() { return parent; }
		vector<shared_ptr<Component>> getChildren() { return children; }
		shared_ptr<Component> getChild(int i);

	private:
		bool isActive = true;
		ofRectangle rect;
		float scale = 1.0;
		float rotation = 0;
		ofMatrix4x4 localMatrix, localMatrixInverse, globalMatrix, globalMatrixInverse;
		void updateMatrix();
		void updateGlobalMatrix();

		bool draggable = false, dragging = false;

		shared_ptr<Component> parent = nullptr;
		vector<shared_ptr<Component>> children;
	};
}