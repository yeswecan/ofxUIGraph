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
		bgColor = ofColor(0);
		textColor = ofColor(255);
		fillColor = ofColor(127);
		font = NULL;
    	init();
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
//            ofLog() << "FOCUSING ON ME";
            if (focusedObject != this) cursorPosition = innards.length();
            focusOnObject(this);
            return true;
        };
        
        draw = [&](UIObject *o) {

            ofSetColor(bgColor);
            ofRect(0,0,o->size.x, o->size.y);
            
			ofSetColor(fillColor);
			ofRect(0, 0, o->size.x * (float)ofToFloat(innards) / (float)maxValue, o->size.y);
            
            ofSetColor(textColor);
            string whatToDisplay = innards;
            if (showAsInt) {
                whatToDisplay = ofToString(ofToInt(innards));
            }
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

            // the following glBlendFunc enables background inversion, but doesn't work here
            // for some reason :/
//            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
            if (font == NULL) {
                ofSetColor(0);
                ofDrawBitmapString(whatToDisplay, 15, 17);
                ofSetColor(230);
                ofDrawBitmapString(whatToDisplay, 15, 15);
            } else {
                ofSetColor(0);
                font->drawString(whatToDisplay, 17, font->getLineHeight());
                ofSetColor(230);
                font->drawString(whatToDisplay, 15, font->getLineHeight());
            }
            ofEnableAlphaBlending();
            
        };
        
		onValueChange = [&](EditableField*){};
		onEnter = [&](EditableField*){};
        
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
    
    EditableField(ofPoint position_, ofPoint size_) {
        bgColor = ofColor(0);
        font = NULL;
        
        position = position_;
        size = size_;
        
        init();
    }

    EditableField(ofPoint position_, ofPoint size_, float minimumValue, float maximumValue, bool isInt = false) {
        bgColor = ofColor(0);
        font = NULL;
        
        position = position_;
        size = size_;
        
        minValue = minimumValue;
        maxValue = maximumValue;
        
        init();
        
        numbersOnly = true;
        setupForDragging();
        
        innards = ofToString(minimumValue);
        
        showAsInt = isInt;
    }
    
    float getAsFloat() {
        return ofToFloat(innards);
    }


    float getAsInt() {
        return ofToInt(innards);
    }

	void clamp() {
	   if (ofToFloat(innards) < minValue) innards = ofToString(minValue);
      if (ofToFloat(innards) > maxValue) innards = ofToString(maxValue);
      cursorPosition = innards.length();
	}
    
    void setupForDragging() {
        UIDragGestureRecognizer *dgr = new UIDragGestureRecognizer();
        
        dgr->gestureStarted = [&](DragGestureArgs args) {
            numWhenDragStarted = ofToFloat(innards);
            int df = ((UIDragGestureRecognizer*)gestureRecognizer)->dragFinger;
            fingerPositionWhenDragStarted = UIObject::fingerPositions[df];
        };
        
        dgr->gestureUpdated = [&](DragGestureArgs args) {
            UIDragGestureRecognizer *dgr = args.recognizer;
            if (dgr->dragFinger != 0) return; //hmmm
            //int diff = (UIObject::fingerPositions[dgr->dragFinger] - fingerPositionWhenDragStarted).y;
            float diff = (((fingerPositionWhenDragStarted - UIObject::fingerPositions[dgr->dragFinger]).x)
                          / size.x) * (maxValue - minValue);
            
            this->innards = ofToString(numWhenDragStarted - diff);
            this->clamp();
            
            onValueChange(this);
            
        };
        
        dgr->gestureEnded = [&](DragGestureArgs args) {
        };
        
        gestureRecognizer = dgr;
        
        if (innards == "") innards = "0"; // what the fuck is this
    }
    
    void setupForDragging(float min, float max) {
        numbersOnly = true;
        
        minValue = min;
        maxValue = max;
        
        setupForDragging();
    }
    
    void insertSymbol(char input) {
//        ofLog() << "inserting " << input;
        innards.insert(innards.begin() + cursorPosition, input);
        cursorPosition++;
        
        /*if (innards[0] == '0') {
            innards.erase(0, 1);
            cursorPosition--;
        }*/
        this->clamp();
    }
    
    void keyPressed(int key) {
//        ofLog() << "key pressed " << key;
        
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
            onValueChange(this);
        }
        
        if (numbersOnly) {
            if ((( key >= 48 )&&( key <= 57 )) || key == '.' || key == '-') { //number keys
                insertSymbol((char)key);
            	onValueChange(this);
            }
        } else
        if (((key >= 45) && (key <= 122)) || (key == ' ')) { // numbers and letters
            insertSymbol((char)key);
            onValueChange(this);
        }
        if (key == 13) {
            unfocusKeyboardRecievers();
            onEnter(this);
        }
    }

	int getValue() {
		return ((int)ofToFloat(innards));
	}
	float getValuef() {
		return (ofToFloat(innards));
	}

    void setValue(float value) {
        innards = ofToString((float)value);
    }
    
    void setValue(int value) {
        innards = ofToString((float)value);
    }
    
    bool showAsInt = false;
    
    function<void(EditableField*)> onValueChange;
    function<void(EditableField*)> onEnter;
    
    bool numbersOnly;
    string innards;
    float numWhenDragStarted, minValue, maxValue;
    ofPoint fingerPositionWhenDragStarted;
    
    ofColor bgColor, textColor, strokeColor, fillColor;
    ofTrueTypeFont *font;
    
    int cursorPosition = 0;
};

#endif /* EditableField_h */
