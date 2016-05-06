//
//  EditableField.h
//  uiGraphTest
//
//  Created by zebra on 04.11.15.
//
//

#ifndef EditableField_h
#define EditableField_h

#include "UIObject.h"
#include "UIKeyboardEventReciever.h"

using namespace UIGraph;

class EditableField: public UIObject, UIKeyboardEventReciever {
public:
    EditableField() {
        bgColor = ofColor(0);
        textColor = ofColor(255);
		fillColor = ofColor(127);
        font = NULL;

        init();
        
    }
    EditableField(string nam) {
    	EditableField();
		name = nam;
    }
    
    
    float getCursorVisualPosition() {
        while (cursorPosition > innards.length()) {cursorPosition--;}
        
        if (cursorPosition == 0) return 0;

        if (font == NULL) return 8 * cursorPosition;
        
        string temp = innards;
        temp.erase(cursorPosition, innards.length() - cursorPosition);
        return font->getStringBoundingBox(temp, 0, 0).width;
    }
    
    void init() {
        useFbo = true;
        
        touchDownC = [&](UIObject *o)->bool {
            ofLog() << "FOCUSING ON ME";
            if (focusedObject != this) cursorPosition = innards.length();
            focusOnObject(this);
            return true;
        };
        
        draw = [&](UIObject *o) {
            
            ofSetColor(bgColor);
            ofRect(0,0,o->size.x, o->size.y);
            
			ofSetColor(fillColor);
			ofRect(0, 0, o->size.x * (float)ofToInt(innards) / (float)maxValue, o->size.y);
            
            ofSetColor(textColor);
            string whatToDisplay = innards;
            if (getFocusedObject() == this) {
                if ((ofGetElapsedTimef() - (int)ofGetElapsedTimef()) < 0.5) {
                    ofLine(15 + getCursorVisualPosition() + 2, 2, 15 + getCursorVisualPosition() + 2, size.y - 4);
                }
                
                ofSetColor(strokeColor);
                ofNoFill();
                    ofSetLineWidth(3);
                    ofRect(0, 0, o->size.x, o->size.y);
                    ofSetLineWidth(1);
                ofFill();
            }
            if (font == NULL) {
                ofDrawBitmapString(whatToDisplay, 15, 15);
            } else {
                font->drawString(whatToDisplay, 15, font->getLineHeight());
            }
            
        };
        
		onValueChange = [&](){};
		onEnter = [&](){};
        
        strokeColor = ofColor(230);
    }
    
    EditableField(string name_, ofPoint position_, ofPoint size_) {
        bgColor = ofColor(0);
        font = NULL;

        name = name_;
        position = position_;
        size = size_;
        
        init();
    }
    
    void setupForDragging(int min, int max) {
        numbersOnly = true;
        
        minValue = min;
        maxValue = max;
        
        UIDragGestureRecognizer *dgr = new UIDragGestureRecognizer();
        
        dgr->gestureStarted = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
            numWhenDragStarted = ofToInt(innards);
            fingerPositionWhenDragStarted = UIObject::fingerPositions[dgr->dragFinger];
        };
        
        dgr->gestureUpdated = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
			if (dgr->dragFinger != 0) return; //hmmm
            //int diff = (UIObject::fingerPositions[dgr->dragFinger] - fingerPositionWhenDragStarted).y;
			int diff = (fingerPositionWhenDragStarted - UIObject::fingerPositions[dgr->dragFinger]).x;
            this->innards = ofToString(numWhenDragStarted - diff);
            if (ofToInt(innards) < minValue) innards = ofToString(minValue);
            if (ofToInt(innards) > maxValue) innards = ofToString(maxValue);
            cursorPosition = innards.length();
            
            onValueChange();
            
        };
        dgr->gestureEnded = [&](UIGestureRecognizer *d) {
        };
        
        gestureRecognizer = dgr;
        
        if (innards == "") innards = "0";
    }
    
    void insertSymbol(char input) {
        ofLog() << "inserting " << input;
        innards.insert(innards.begin() + cursorPosition, input);
        cursorPosition++;
        
        if (innards[0] == '0') {
            innards.erase(0, 1);
            cursorPosition--;
        }
    }
    
    void keyPressed(int key) {
        ofLog() << "key pressed " << key;
        
        if (key == OF_KEY_LEFT) {
            if (cursorPosition > 0) cursorPosition--;
        }

        if (key == OF_KEY_RIGHT) {
            if (cursorPosition < innards.length()) cursorPosition++;
        }
        
        if (key == OF_KEY_BACKSPACE) { // backspace
            if ((innards.length() > 0) && (cursorPosition > 0)) {
                innards.erase(cursorPosition - 1, 1);
                cursorPosition--;
            }
            onValueChange();
        }
        
        if (numbersOnly) {
            if (( key >= 48 )&&( key <= 57 )) { //number keys
                insertSymbol((char)key);
            }
            onValueChange();
        } else
        if (((key >= 45) && (key <= 122)) || (key == ' ')) { // numbers and letters
            insertSymbol((char)key);
            onValueChange();
        }
        if (key == 13) {
            unfocusKeyboardRecievers();
            onEnter();
        }
    }

	int getValue() {
		return (ofToInt(innards));
	}
    
    void setValue(float value) {
        innards = ofToString((int)value);
    }
    
    void setValue(int value) {
        innards = ofToString((int)value);
    }
    
    
    function<void()> onValueChange;
    function<void()> onEnter;
    
    bool numbersOnly;
    string innards;
    int numWhenDragStarted, minValue, maxValue;
    ofPoint fingerPositionWhenDragStarted;
    
    ofColor bgColor, textColor, strokeColor, fillColor;
    ofTrueTypeFont *font;
    
    int cursorPosition = 0;
};

#endif /* EditableField_h */
