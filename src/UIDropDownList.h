//
//  UIDropDownList.h
//  uiGraphTest
//
//  Created by zebra on 04.11.15.
//
//

#ifndef UIDropDownList_h
#define UIDropDownList_h

#include "UIObject.h"
#include "UIKeyboardEventReciever.h"

class UIDropDownList: public UIObject, public UIKeyboardEventReciever {
public:
    UIDropDownList(string name, vector<string> *listValues) {
        this->values = listValues;
        
        rowHeight = 20;
        
        useFbo = true;
        
        update = [&](UIObject *o) {
            if (!initialized) {
                parent->addChild(openedDropDownList);
                openedDropDownList->visible = false;
                initialized = true;
            }
            
            if ((UIKeyboardEventReciever::getFocusedObject() != this) &&
                (openedDropDownList->visible))
                {
                openedDropDownList->visible = false;
            }
        };
        
        draw = [&](UIObject *o) {
            ofSetColor(0, 0, 0);
            ofRect(0, 0, o->size.x, o->size.y);
            
            ofSetColor(255);
            ofDrawBitmapString(ofToString(values->at(selectedRow)), 15, 15);
        };
        
        touchDownC = [&](UIObject *o)->bool {
            openedDropDownList->visible = true;
            
            if (UIKeyboardEventReciever::getFocusedObject() != this) {
                UIKeyboardEventReciever::focusOnObject(this);
            } else {
                UIKeyboardEventReciever::unfocusKeyboardRecievers();
            }
        };
        
        
        ///
        openedDropDownList = new UIObject("oddl");
        openedDropDownList->useFbo = true;
        openedDropDownList->setFboSize(ofPoint(75, 200));
        ConstraintSolver::addPositionConstraint(
                    openedDropDownList, UIConstraint2D(
                                UIConstraint(UIConstraint::PARALLEL_TO_LEFT, this, openedDropDownList, 0),
                                UIConstraint(UIConstraint::PARALLEL_TO_TOP, this, openedDropDownList, 0)                       ));
        ConstraintSolver::addSizeConstraint(openedDropDownList, UIConstraint2D(
                        UIConstraint(UIConstraint::PERCENTAGE_OF_SIZE_X, this, 1),
                                                            UIConstraint(200)));
        
        openedDropDownList->update = [&](UIObject *o) {
            float maximum = rowHeight * values->size() - openedDropDownList->size.y;
//            o->innerTransform = ofPoint(0, -(maximum * 0.5 + sin(ofGetElapsedTimef()) * maximum * 0.5));
        };
        
        
        openedDropDownList->innerTransform = ofPoint(0, -50);
        
        openedDropDownList->draw = [&](UIObject *o) {
            ofPushMatrix();
            
            float maximum = 20 * values->size() - openedDropDownList->size.y;
            // TODO:
            // 1) translate to the point where the selected object lies
            //    when you use keys

            ofTranslate(0, o->innerTransform.y);
            
//            ofTranslate(o->innerTransform);
            
//            ofRect(0, 0, o->size.x, o->size.y);
            
            for (int i = 0; i < values->size(); i++) {
                ofSetColor(0, 0, 0, 200);
                ofRect(0, (i) * rowHeight, o->size.x, rowHeight);
                ofSetColor(255);
                ofDrawBitmapString(ofToString(i) + ". " + values->at(i), rowHeight, (i + 1) * rowHeight - 3);
                if (selectedRow == i) {
                    ofSetColor(255);
                    ofRect(0, (i) * rowHeight, o->size.x, rowHeight);
                    ofSetColor(0);
                    ofDrawBitmapString(values->at(i), rowHeight + 5, (i + 1) * rowHeight - 3);
                }
            }
            
            ofPopMatrix();

            if (maximum > openedDropDownList->size.y) {
                // drawing scroll bar
                
                float size = (openedDropDownList->size.y / maximum) * openedDropDownList->size.y;
                float offset = -(openedDropDownList->innerTransform.y / (maximum)) * (openedDropDownList->size.y - size);

                ofRect(openedDropDownList->size.x - 20, offset, 20, size);
            };
            
        };
        
        openedDropDownList->touchDownC = [&](UIObject *o)->bool {
            return true;
        };
        
        // Scroll bar dragging
        UIDragGestureRecognizer *dgr = new UIDragGestureRecognizer();
        dgr->gestureStarted = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;

            initialInnerTransform = openedDropDownList->innerTransform;
            initialTouchPosition = UIObject::fingerPositions[dgr->dragFinger] - dgr->dragOffset - openedDropDownList->position;
            if (dgr->dragOffset.x < (openedDropDownList->size.x - 20)) {
                dgr->stop();
                int pos = -(openedDropDownList->innerTransform.y - dgr->dragOffset.y) / rowHeight;
                selectedRow = pos;
                UIKeyboardEventReciever::unfocusKeyboardRecievers();
                openedDropDownList->visible = false;
            } else {
                ofLog() << "NOT DROPPING";
            }
        };
        dgr->gestureUpdated = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
            float maximum = rowHeight * values->size() - openedDropDownList->size.y;
            ofPoint offset = UIObject::fingerPositions[dgr->dragFinger] - dgr->dragOffset - openedDropDownList->position - ofPoint(rowHeight);
//            ofLog() << (initialTouchPosition - offset);
            
            float moveAmount = offset.y * (maximum / openedDropDownList->size.y);
            float newInnerTransform = initialInnerTransform.y - moveAmount;
            openedDropDownList->innerTransform.y =  newInnerTransform;
            if (openedDropDownList->innerTransform.y < -maximum) openedDropDownList->innerTransform.y = -maximum;
            if (openedDropDownList->innerTransform.y > 0) openedDropDownList->innerTransform.y = 0;
//
//            if (o4->position.x < 0) o4->position.x = 0;
//            if (o4->position.y < 0) o4->position.y = 0;
            
        };
        dgr->gestureEnded = [&](UIGestureRecognizer *d) {
            ofLog() << "gesture ended!";
        };
        
        openedDropDownList->gestureRecognizer = dgr;
        
    }
    
    void lostFocus() {
        openedDropDownList->visible = false;
    }
    
    void keyPressed(int key) {
        ofLog() << "dropdown recieved " << key;
        
        // TODO:
        // move the scroll view to the point where it can be seen
        if (key == 357) {
            if (selectedRow >= 1)
                selectedRow --;
        }
        if (key == 359) {
            if (selectedRow < values->size()-1)
                selectedRow ++;
        }
        if (key == 13) {
            UIKeyboardEventReciever::unfocusKeyboardRecievers();
//            openedDropDownList->visible = false;
        }
    };

    
    bool initialized = false;
    
    vector<string> *values;
    int selectedValueIndex;
    int rowHeight;
    
    ofPoint initialInnerTransform, initialTouchPosition;
    
    int selectedRow = 5;
    
    UIObject *openedDropDownList;
};

#endif /* UIDropDownList_h */
