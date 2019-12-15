#pragma once
#include "ofxComponentBase.h"

namespace ofxComponent {
	class ofxComponentManager : public ofxComponentBase {
	public:
		ofxComponentManager();
		~ofxComponentManager();

		void onSetup();
		void update(ofEventArgs &);
		void draw(ofEventArgs &);
		void exit(ofEventArgs &);
		void keyPressed(ofKeyEventArgs &);
		void keyReleased(ofKeyEventArgs &);
		void mouseMoved(ofMouseEventArgs &);
		void mousePressed(ofMouseEventArgs &);
		void mouseDragged(ofMouseEventArgs &);
		void mouseReleased(ofMouseEventArgs &);
		void dragEvent(ofDragInfo &);
	};
}