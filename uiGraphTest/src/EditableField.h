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
        init();
    }
    
    void init() {
        useFbo = true;
        
        touchDownC = [&](UIObject *o)->bool {
            ofLog() << "FOCUSING ON ME";
            focusOnObject(this);
            return true;
        };
        
        draw = [&](UIObject *o) {
            ofSetColor(0);
            if (getFocusedObject() == this) {
                ofSetColor(100);
            }
            ofRect(0,0,o->size.x, o->size.y);
            
            
            ofSetColor(255);
            ofDrawBitmapString(innards, 15, 15);
        };
        
    }
    
    EditableField(string name_, ofPoint position_, ofPoint size_) {
        name = name_;
        position = position_;
        size = size_;
        
        init();
    }
    
    void setupForDragging(int min, int max) {
        numbersOnly = true;
        
        minNum = min;
        maxNum = max;
        
        UIDragGestureRecognizer *dgr = new UIDragGestureRecognizer();
        dgr->gestureStarted = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
            numWhenDragStarted = ofToInt(innards);
            fingerPositionWhenDragStarted = UIObject::fingerPositions[dgr->dragFinger];
        };
        dgr->gestureUpdated = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
            int diff = (UIObject::fingerPositions[dgr->dragFinger] - fingerPositionWhenDragStarted).y;
            this->innards = ofToString(numWhenDragStarted - diff);
            if (ofToInt(innards) < minNum) innards = ofToString(minNum);
            if (ofToInt(innards) > maxNum) innards = ofToString(maxNum);
            
            onNumberChange();
//            o4->position = UIObject::fingerPositions[dgr->dragFinger] - o3->position - dgr->dragOffset;
//            
//            if (o4->position.x < 0) o4->position.x = 0;
            
        };
        dgr->gestureEnded = [&](UIGestureRecognizer *d) {
        };
        
        gestureRecognizer = dgr;
        
    }
    
    void keyPressed(int key) {
        if (key == 127) { // backspace
            if (innards.length() > 0) {
                innards.erase(innards.length() - 1, 1);
            }
            onNumberChange();
        }
        
        if (numbersOnly) {
            if (( key >= 48 )&&( key <= 57 )) { //number keys
                innards += (char)key;
            }
            onNumberChange();
        } else
        if ((key >= 45) && (key <= 122)) { // numbers and letters
            innards += (char)key;
            onNumberChange();
        }
    }
    
    
    function<void()> onNumberChange = [&](){};
    
    bool numbersOnly;
    string innards;
    int numWhenDragStarted, minNum, maxNum;
    ofPoint fingerPositionWhenDragStarted;
};

#endif /* EditableField_h */
