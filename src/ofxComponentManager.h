#pragma once
#include "ofxComponentBase.h"

namespace ofxComponent {
	class ofxComponentManager : public ofxComponentBase {
	public:
		ofxComponentManager();
		~ofxComponentManager();

		void onSetup() override;
		void update(ofEventArgs &);
		void draw(ofEventArgs &);
		void exit(ofEventArgs &);
		void keyPressed(ofKeyEventArgs &);
		void keyReleased(ofKeyEventArgs &);
		void mouseMoved(ofMouseEventArgs &);
		void mousePressed(ofMouseEventArgs &);
		void mouseDragged(ofMouseEventArgs &);
		void mouseReleased(ofMouseEventArgs &);
		void mouseScrolled(ofMouseEventArgs &);
		void dragEvent(ofDragInfo &);
        
        static shared_ptr<ofxComponentManager> singleton;
    private:
        void removeDestroyedObjects();
	};
}
