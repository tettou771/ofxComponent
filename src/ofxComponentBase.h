#pragma once
#include "ofMain.h"

#ifndef TAU
#define TAU 6.28318530717958647693
#endif
#ifndef HALF_TAU
#define HALF_TAU 3.14159265358979323846
#endif

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

        // This method is called when the mouse is pressed down while over the topmost visible component, even if multiple components are overlapping.
        // It handles the actions or events that should occur in response to the mouse press on that topmost component.
        // This ensures that only the foremost layer receives the event, making it particularly useful for triggering events, such as button clicks or starting a drag action, when interacting with overlapping UI components.
        virtual void onMousePressedOverComponent(ofMouseEventArgs&){}

        // is mouse pointer on this
        // When using this for creating UI elements, there may be issues with multiple overlapping buttons.
        // Consider using isMouseOver() as well.
        bool isMouseInside();

        // isMouseOver() returns true when the mouse pointer is hovering over an object and the object is located on the top layer.
        // When implementing UI elements such as buttons, if multiple buttons overlap, using isMouseInside() alone may cause multiple objects to respond.
        // To prevent this, by also checking whether the object is on the top layer, simultaneous pressing of overlapping buttons can be avoided.
        // In addition, when implementing buttons, another option is to register events with mousePressedTopEvents.
        bool isMouseOver();
        
        // This method returns true while the mouse is clicked and held over the component.
        // It continues to return true even if the mouse is dragged outside of the component.
        // This behavior is useful for recognizing mouse movements when dragging a component, even when outside of its bounds.
        bool isMousePressedOverComponent();

        // key and mouse enabled
		void setKeyMouseEventEnabled(bool enabled);
		bool toggleKeyMouseEventEnabled();
		bool getKeyMouseEventEnebled();

		// On change position, size, scale, etc
		virtual void onLocalMatrixChanged() {};
		// Notify to others when localMatrixChanged.
		ofEvent<void> localMatrixChangedEvents;

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
		void setMovable(bool _movable);
		bool getMovable();
		void setMoving(bool _moving);
		bool getMoving();
		bool inside(ofVec2f p);
		bool inside(float x, float y);
        
		// parent/child control
		void setParent(shared_ptr<ofxComponentBase> _parent);
		void removeParent();
		void addChild(shared_ptr<ofxComponentBase> _child);
		void insertChild(shared_ptr<ofxComponentBase> _child, int index);
		void removeChild(shared_ptr<ofxComponentBase> _child);
        void swapChild(int indexA, int indexB);

        // Set constrain to true when you want to draw child components only within the size defined by the rect of the parent component.
        // Internally, an FBO is created and used to draw the contents, which are then redrawn, resulting in slightly increased memory usage.
        // This can be used, for example, when implementing a window and you want to draw child components only within the window.
        // onMousePressed() collider will also be constrained within the bounds of the parent component.
		void setConstrain(bool _constrain);
		bool getConstrain();

		// remove this
		void destroy();
		void destroy(float delaySec); // delay destroy [sec]
		bool isDestroyed();

		// component getter
		shared_ptr<ofxComponentBase> getParent() { return parent; }
		vector<shared_ptr<ofxComponentBase>> getChildren() { return children; }
		shared_ptr<ofxComponentBase> getChild(int i);
        
        // get shared_ptr
        shared_ptr<ofxComponentBase> getThis() {
            return shared_from_this();
        }
        template<typename T>
        inline shared_ptr<T> getThisAs() {
            return dynamic_pointer_cast<T>(shared_from_this());
        }
        
        // This is triggered when a button is pressed and the button is located at the top layer at that coordinate.
        // In this case, use ofAddListener() and provide the instance of this class's mousePressedTopEvents as the first argument.
        ofEvent<void> mousePressedOverComponentEvents;

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

		// constrain draw in the rect
		bool constrain = false;
		ofFbo constrainFbo;

		bool needStartExec = true;
		bool movable = false;
        bool mousePressedOverComponent = false;
		bool destroyed = false;
		static shared_ptr<ofxComponentBase> movingComponent;

		shared_ptr<ofxComponentBase> parent = nullptr;
		vector<shared_ptr<ofxComponentBase>> children;

    public:
        typedef function<void()> TimerFunc;

        class Timer {
		public:
			Timer(TimerFunc func, float wait);
			TimerFunc function;
			float execTime;
			bool canceled = false;
		};

    protected:
		vector<Timer*> timerFunctions;

	public:
		Timer* addTimerFunction(TimerFunc func, float wait);
		void clearTimerFunctions();

	protected:
		static vector<shared_ptr<ofxComponentBase> > allComponents;
		static vector<shared_ptr<ofxComponentBase> > destroyedComponents;
        static shared_ptr<ofxComponentBase> mouseOverComponent;
	};
}
