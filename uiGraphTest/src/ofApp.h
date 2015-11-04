#pragma once

#include "ofMain.h"
#include "ofxUIGraph.h"
#include "UIDropDownList.h"
#include "EditableField.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
    UIObject *ui;
    UIObject *o1, *o2, *o3, *o4;
    
    UIConstraint2D p1, s1, p2, s2;
    
    EditableField *e, *e2;
};
