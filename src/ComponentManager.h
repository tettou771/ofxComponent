#pragma once
#include "Component.h"

namespace ofxComponent {
	class ComponentManager : public Component {
	public:
		ComponentManager();
		~ComponentManager();

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
	};
}