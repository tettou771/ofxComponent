#pragma once
#include "ofMain.h"

namespace ofxComponent {
	enum Alignment {
		Cornar,
		Center
	};

	class ofxComponentBase : public enable_shared_from_this<ofxComponentBase> {
	public:
		ofxComponentBase();
		~ofxComponentBase();

		// of events
		void setup();
		virtual void onSetup() {}

		void update(ofEventArgs&);
		virtual void onUpdate() {}
		virtual void postUpdate() {}

		void draw(ofEventArgs&);
		virtual void onDraw() {}
		virtual void postDraw() {}

		void exit(ofEventArgs&);
		virtual void onExit() {}

		// call before first update
		void start();
		virtual void onStart() {}

		// call on destroy
		virtual void onDestroy() {}

		// acitive / disactive
		void setActive(bool);
		bool getActive() { return isActive; }
		virtual void onActiveChanged(bool active) { ; }

		// global active
		bool getGlobalActive();
		virtual void onGlobalActiveChanged(bool active) { ; }

		// key
		void keyPressed(ofKeyEventArgs&);
		virtual void onKeyPressed(ofKeyEventArgs&) {}

		void keyReleased(ofKeyEventArgs&);
		virtual void onKeyReleased(ofKeyEventArgs&) {}

		// mouse
		void mouseMoved(ofMouseEventArgs&);
		virtual void onMouseMoved(ofMouseEventArgs&) {}

		void mousePressed(ofMouseEventArgs&);
		virtual void onMousePressed(ofMouseEventArgs&) {}

		void mouseDragged(ofMouseEventArgs&);
		virtual void onMouseDragged(ofMouseEventArgs&) {}

		void mouseReleased(ofMouseEventArgs&);
		virtual void onMouseReleased(ofMouseEventArgs&) {}

		void mouseScrolled(ofMouseEventArgs&);
		virtual void onMouseScrolled(ofMouseEventArgs&) {};

		void dragEvent(ofDragInfo&);
		virtual void onDragEvent(ofDragInfo&) {}

		// key and mouse enabled
		void setKeyMouseEventEnabled(bool enabled);
		bool toggleKeyMouseEventEnabled();
		bool getKeyMouseEventEnebled();

		virtual void onLocalMatrixChanged() {};

		ofRectangle getRect();
		ofRectangle getGlobalRect();
		ofVec2f getPos();
		ofVec2f getCenterPos();
		ofVec2f getGlobalPos();
		ofVec2f getGlobalCenterPos();
		Alignment getScaleAlignment();
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
		void setScaleAlignment(Alignment);
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
		void setDragging(bool _dragging);
		bool getDragging();
		bool inside(ofVec2f p);
		bool inside(float x, float y);
		bool isMouseInside();

		// parent/child control
		void setParent(shared_ptr<ofxComponentBase> _parent);
		void removeParent();
		void addChild(shared_ptr<ofxComponentBase> _child);
		void insertChild(shared_ptr<ofxComponentBase> _child, int index);
		void removeChild(shared_ptr<ofxComponentBase> _child);
        void swapChild(int indexA, int indexB);

		// remove this
		void destroy();
		void destroy(float delaySec); // delay destroy [sec]
		bool isDestroyed();

		// component getter
		shared_ptr<ofxComponentBase> getParent() { return parent; }
		vector<shared_ptr<ofxComponentBase>> getChildren() { return children; }
		shared_ptr<ofxComponentBase> getChild(int i);

	private:
		bool isActive = true;
		bool keyMouseEventEnabled = true;
		ofRectangle rect;
		float scale = 1.0;
		Alignment scaleAlignment = Center;
		float rotation = 0;
		ofMatrix4x4 localMatrix, localMatrixInverse, globalMatrix, globalMatrixInverse;
		void updateMatrix();
		void updateGlobalMatrix();
		void globalActiveChanged(bool _globalActive);

		bool needStartExec = true;
		bool draggable = false;
		bool destroyed = false;
		static shared_ptr<ofxComponentBase> draggingComponent;

		shared_ptr<ofxComponentBase> parent = nullptr;
		vector<shared_ptr<ofxComponentBase>> children;

	private:
		typedef function<void()> TimerFunc;

		class Timer {
		public:
			Timer(TimerFunc func, float wait);
			TimerFunc function;
			float execTime;
			bool canceled = false;
		};

		vector<Timer*> timerFunctions;

	public:
		void addTimerFunction(TimerFunc func, float wait);
		void clearTimerFunctions();

	protected:
		static vector<shared_ptr<ofxComponentBase> > allComponents;
		static vector<shared_ptr<ofxComponentBase> > destroyedComponents;
	};
}
