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

class UIDropDownListDataProvider {
public:
	virtual const string at(int index) = 0;
    
	virtual const int size() = 0;
};

class UIDropDownList: public UIObject, public UIKeyboardEventReciever {
public:
    
    UIDropDownList(string name) : UIObject(name) {
        rowHeight = 20;
        
        useFbo = false;
        
        name = "dropdown";
        
        selectedNewRow = []{};
        font = NULL;
        fontOffset = ofPoint(0);
        
        showIcon = dropdownIcon.loadImage("assets/dropdown_icon.png");
        
        
        update = [&](UIObject *o) {
            if (!initialized) {
                ofLog() << "parent name:" << parent->name;
                for (int i = 0; i < parent->children.size(); i++) {
                    ofLog() << i << "." << parent->children[i]->name;
                }
                
                parent->addChild(openedDropDownList);
                
                for (int i = 0; i < parent->children.size(); i++) {
                    ofLog() << i << "." << parent->children[i]->name;
                }
                
                ofLog() << "top parent name = " << getTopParent()->name;
                getTopParent()->addCallback(TOUCH_DOWN, [this]() {
                        if ((this->timeFromEvent(TOUCH_DOWN) > 50) && (this->openedDropDownList->timeFromEvent(TOUCH_DOWN) > 50)) {
                            if ((UIKeyboardEventReciever::getFocusedObject() == this) &&
                                (openedDropDownList->visible))
                            {
                                UIKeyboardEventReciever::unfocusKeyboardRecievers();
                            }
                        }
                    } );
                
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
            
            ofPushMatrix();
            ofSetColor(255);
            if (font == NULL) {
                ofDrawBitmapString(ofToString(getValueAt(selectedRow)), 15, 15);
            } else {
                font->drawString(getValueAt(selectedRow), 15 + fontOffset.x, 15 + fontOffset.y);
            }
            ofPopMatrix();
            if (!openedDropDownList->visible)
                if (showIcon) dropdownIcon.draw(o->size.x - 5 - dropdownIcon.getWidth(), o->size.y / 2 - dropdownIcon.getHeight() / 2);
        };
        
        touchDownC = [&](UIObject *o)->bool {
            openedDropDownList->visible = true;
            
            if (UIKeyboardEventReciever::getFocusedObject() != this) {
                UIKeyboardEventReciever::focusOnObject(this);
            } else {
                UIKeyboardEventReciever::unfocusKeyboardRecievers();
            }
            
            return true;
        };
        
        
        ///
        openedDropDownList = new UIObject("oddl");
        openedDropDownList->useFbo = true;
        openedDropDownList->setFboSize(ofPoint(75, 200));
        UIConstraintSolver::addPositionConstraint(
                                                openedDropDownList, UIConstraint2D(
                                                                                   UIConstraint(UIConstraint::PARALLEL_TO_LEFT, this, openedDropDownList, 0),
                                                                                   UIConstraint(UIConstraint::PARALLEL_TO_TOP, this, openedDropDownList, 0)                       ));
        UIConstraintSolver::addSizeConstraint(openedDropDownList, UIConstraint2D(
                                                                               UIConstraint(UIConstraint::PERCENTAGE_OF_SIZE_X, this, 1),
                                                                               UIConstraint(200)));
        
        openedDropDownList->update = [&](UIObject *o) {
            float maximum = rowHeight * getRowsCount() - openedDropDownList->size.y;
        };
        
        
        openedDropDownList->innerTransform = ofPoint(0, 0);
        
        openedDropDownList->draw = [&](UIObject *o) {
            ofPushMatrix();
            
            float maximum = rowHeight * getRowsCount() - openedDropDownList->size.y;
            // TODO:
            // 1) translate to the point where the selected object lies
            //    when you use keys
            
            ofTranslate(0, o->innerTransform.y);
            
            
            for (int i = 0; i < getRowsCount(); i++) {
                ofSetColor(0, 0, 0, 200);
                ofRect(0, (i) * rowHeight, o->size.x, rowHeight);
                ofSetColor(255);
                drawStringWithSelectedFont(ofToString(i) + ". " + getValueAt(i), rowHeight, (i + 1) * rowHeight - 2);
                if (selectedRow == i) {
                    ofSetColor(230);
                    ofRect(0, (i) * rowHeight, o->size.x, rowHeight);
                    ofSetColor(0);
                    drawStringWithSelectedFont(ofToString(i) + ". " + getValueAt(i), rowHeight, (i + 1) * rowHeight - 8);
                }
            }
            
            ofPopMatrix();
            
            if ((rowHeight * getRowsCount()) > openedDropDownList->size.y) {
                // drawing scroll bar
                
                float size = ( openedDropDownList->size.y / (rowHeight * getRowsCount()) ) * openedDropDownList->size.y;
                float cOffset = -(openedDropDownList->innerTransform.y / (maximum)) * (openedDropDownList->size.y - size);
                
                ofRect(openedDropDownList->size.x - 20, cOffset, 20, size);
            }
        };
        
        openedDropDownList->touchDownC = [&](UIObject *o)->bool {
            return true;
        };
        
        // Scroll bar dragging
        UIDragGestureRecognizer *dgr = new UIDragGestureRecognizer();
        dgr->gestureStarted = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
            
            initialInnerTransform = openedDropDownList->innerTransform;
            initialTouchPosition = UIObject::fingerPositions[dgr->dragFinger] - dgr->dragOffset - openedDropDownList->position + ofPoint(0, 0);
            if (dgr->dragOffset.x < (openedDropDownList->size.x - 20)) {
                // selecting a new row
                dgr->stop();
                int pos = -(openedDropDownList->innerTransform.y - dgr->dragOffset.y) / rowHeight;
                selectedRow = pos;
                UIKeyboardEventReciever::unfocusKeyboardRecievers();
                openedDropDownList->visible = false;
                
                selectedNewRow();
            } else {
                // just going on, drag started
//                ofLog() << "NOT DROPPING";
            }
        };
        dgr->gestureUpdated = [&](UIGestureRecognizer *d) {
            UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
            float maximum = rowHeight * getRowsCount() - openedDropDownList->size.y;
            ofPoint cOffset = UIObject::fingerPositions[dgr->dragFinger] - dgr->dragOffset - openedDropDownList->getTransformedPosition();
            ofLog() << (dgr->offset);
            
            float moveAmount = cOffset.y * ((rowHeight * getRowsCount()) / openedDropDownList->size.y);
            float newInnerTransform = initialInnerTransform.y - moveAmount;
            openedDropDownList->innerTransform.y =  newInnerTransform;
            if (openedDropDownList->innerTransform.y < -maximum) openedDropDownList->innerTransform.y = -maximum;
            if (openedDropDownList->innerTransform.y > 0) openedDropDownList->innerTransform.y = 0;
        };
        dgr->gestureEnded = [&](UIGestureRecognizer *d) {
            ofLog() << "gesture ended!";
        };
        
        openedDropDownList->gestureRecognizer = dgr;
        initialized = false;
        selectedRow = 5;
    }
    
    UIDropDownList(string name, UIDropDownListDataProvider *listValues): UIDropDownList(name) {
        this->values = listValues;

    }

    UIDropDownList(string name, vector<string> *listValues): UIDropDownList(name) {
        innerProvider.innerProviderSource = listValues;
        useInnerProvider = true;
    }
    
    void drawStringWithSelectedFont(string str, float x, float y) {
        if (font == NULL) {
            ofDrawBitmapString(str, x, y);
        } else {
            font->drawString(str, x + fontOffset.x, y + fontOffset.y);
        }

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
            if (selectedRow < getRowsCount()-1)
                selectedRow ++;
        }
        if (key == 13) {
            UIKeyboardEventReciever::unfocusKeyboardRecievers();
            selectedNewRow();
        }
    };

    
    bool initialized;

    // Data providers
    
    int getRowsCount() {
        if (useInnerProvider) return innerProvider.size();
        else return values->size();
    }
    
    string getValueAt(int index) {
        if (useInnerProvider) return innerProvider.at(index);
        else return values->at(index);
    }
    
    UIDropDownListDataProvider *values;
    
    class UIDropDownListInnerDataProvider: public UIDropDownListDataProvider {
    public:
        virtual const string at(int index) { return innerProviderSource->at(index); }
        
        virtual const int size() { return innerProviderSource->size(); }

        vector<string> *innerProviderSource;
    };

    UIDropDownListInnerDataProvider innerProvider;
    bool useInnerProvider = false;
    
    // Variables
    
    int selectedValueIndex;
    int rowHeight;
    
    ofPoint initialInnerTransform, initialTouchPosition;
    
    int selectedRow;
    
    // UI Resources
    
    UIObject *openedDropDownList;
    ofTrueTypeFont* font;
    ofPoint fontOffset;
    ofImage dropdownIcon;
    bool showIcon = false;
    
    // Callbacks
    
    function<void()> selectedNewRow;
};
    
#endif /* UIDropDownList_h */
