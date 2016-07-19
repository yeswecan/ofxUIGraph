#pragma once

#include "ofMain.h"
#include "ofxUIGraph.h"
#include "UIDropDownList.h"
#include "UICanvas.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

    ofTrueTypeFont font;
    
    UICanvas ui;
    UIObject *o1, *o2, *o4, *panel;
    
    UIConstraint2D p1, s1, p2, s2;
    
    EditableField *e, *e2;
    
    UIDropDownList *ddl;
    
    vector<string> dropdownOptions;
    
    ofParameter<string> pp;
};
