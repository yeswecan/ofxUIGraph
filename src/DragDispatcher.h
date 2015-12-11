//
//  dragDispatcher.h
//  uigViewportTest
//
//  Created by zebra on 10/09/14.
//
//

#ifndef uigViewportTest_dragDispatcher_h
#define uigViewportTest_dragDispatcher_h

class dragData {
public:
    dragData() {};
    ofPoint dragOffset, dragOffset2, dragPosition, lastTouchedPosition;
    bool draggingMe = false, dragCandidate = false;
    int dragHoldTime = 500;
    
    function<void(UIObject*)> dragStartCallback = [](UIObject *o) {return;};
    function<void(UIObject*)> dragEndCallback = [](UIObject *o) {return;};
    
    UIObject *obj;
    string name;
    
    bool allowDrag = true;
    
    function<void(UIObject*)> oldTouchDown;
};


class DragDispatcher : public UIObject {
public:
    DragDispatcher() : UIObject() {
        name = "drag_dispatcher";
        update = [&](UIObject *o) {
            DragDispatcher *x = (DragDispatcher*)o;
            
            int counter = 0;
            for (auto &i: objects) {
//                ofLog() << counter << " - managing drag for " << i->obj->name << " (tp is " << i->obj->topParent->name.c_str() << ")";
                x->calcDraggableStuff(i);
                counter++;
            }
            
            for (auto i = 0; i < x->objects.size(); i++) {
                //ofLog() << x->objects[i].obj->report();
            }
        };
    }
    
    void setAllowDrag(UIObject *o, bool allowDrag) {
        for (auto &i: objects) {
            if (i->obj == o) i->allowDrag = allowDrag;
        }
    }
    
    bool isDragAllowed(UIObject *o) {
        for (auto &i: objects) {
            if (i->obj == o) return i->allowDrag;
        }
        return false;
    }
    
    void processDragForObject(UIObject *o, ofPoint dragOffset, ofPoint dragOffset2, ofPoint dragPosition) {
        for (auto &i: objects) {
            if (i->obj == o) {
                ofLog() << "updating object for drag start sometime..." << i->obj->name;
                i->dragOffset = dragOffset;
                i->dragOffset2 = dragOffset2;
                i->dragPosition = dragPosition;
                i->dragCandidate = true;
            }
        }
    };
    
    void setDragStartCallback(UIObject *o, function<void(UIObject*)> func) {
        for (auto &i: objects) {
            if (i->obj == o) {
                i->dragStartCallback = func;
            }
        }
    };
    
    void setDragEndCallback(UIObject *o, function<void(UIObject*)> func) {
        for (auto &i: objects) {
            if (i->obj == o) {
                i->dragEndCallback = func;
            }
        }
    };
    
    void removeDraggable(UIObject *o) {
        for (auto i = 0; i < objects.size(); i++) {
            if (objects[i]->obj == o) {
                objects.erase(objects.begin() + i);
            }
        }
    }
    
    dragData* addDraggable(UIObject *o, int dragHoldTime) {
        
        o->addCallback(UIObject::TOUCH_DOWN,[&](UIObject *o)->void {
            ofLog() << "CALLED TOUCHDOWN FROM DRAGDISPATCHER FOR OBJECT " << o->name << " lastTouchedPosition = " << o->lastTouchedPosition << ". object position = " << o->position;
            processDragForObject(o, o->lastTouchedPosition, o->transformedPosition - o->position, lastTouchedPosition);
        });
        o->touchDown = []()->bool{};
        
        dragData *addon = new dragData(); // is it ok to do this in stack?
        addon->obj = o;
        addon->dragHoldTime = dragHoldTime;
        objects.push_back(addon);

        // backup old function:
//        addon->oldTouchDown = o->touchDownC;
        addon->obj->data["strictinside"] = 0;
        addon->name = o->name;

        return addon;
        //auto back = objects.back();
        
    };
    
    bool isDraggingNow(UIObject *o) {
        for (auto &i: objects) {
            if (i->obj == o) {
                if (i->draggingMe)
                    return true;
                else return false;
            }
        }
    }
    
    /*
    dragData addDraggable(Object *o, int dragHoldTime, bool strict) {
        dragData result = addDraggable(o, dragHoldTime);
        result.obj->data["strictinside"] = 1;
        ofLog() << "added draggable " << result.obj->name;
        return result;
    };*/

    void setStrict(UIObject *o, int snapThreshold) {
        for (auto &i: objects) {
            if (i->obj == o) {
                o->data["snaptoparent"] = snapThreshold;
                o->data["strictinside"] = 1;
            }
        }
    };
    
    void setDragOffset(UIObject *o, ofPoint offset) {
        for (auto &i: objects) {
            if (i->obj == o) {
                i->dragOffset = offset;
            }
        }
    }
    
    void stopDrag(UIObject *o) {
        for (auto &i: objects) {
            if (i->obj == o) {
                if (i->draggingMe) {
                    i->dragEndCallback(i->obj);
                    color = ofPoint(200, 0, 0);
                    
                    ofLog() << "::::CALLING SPACER'S TOUCH UP FUNCS";
                    i->obj->touchUp();
                    i->obj->touchUpC(i->obj);
                    i->obj->executeCallbacks(UIObject::TOUCH_UP, i->obj);
                    
                    i->draggingMe = false;
                    i->dragCandidate = false;                }
            }
        }
    };
    
    // TODO: also we need coords in this, how do you know what finger it was?
    void startDrag(dragData *i) {
            //ofLog() << "fail from " << name;
                processDragForObject(i->obj, i->obj->lastTouchedPosition, i->obj->transformedPosition - i->obj->position, lastTouchedPosition);
                i->draggingMe = true;
                i->dragStartCallback(i->obj);
    };
    
    void calcDraggableStuff(dragData *i) {
//        if (i->dragCandidate) ofLog() << i->name << " is d candidate";
//        if (i->allowDrag) ofLog() << i->name << " allows drag";
        if ((i->dragCandidate) && (i->obj->timeQFromLastTouch(i->dragHoldTime) == 1) && (!i->draggingMe) && (i->allowDrag)) {
            //i.dragPosition = i.lastTouchedPosition;
            
            processDragForObject(i->obj, i->obj->lastTouchedPosition, i->obj->transformedPosition - i->obj->position, lastTouchedPosition);
            i->draggingMe = true;
            ofLog() << "::::CHANGING TO DRAG - " << i->obj->name;
            i->dragStartCallback(i->obj);
            //topParent->focused.push_back(i.obj);
        }
        //lastMessage = "draggable";
        if (i->draggingMe) {
            //int test = ofRandom(1000);
            //ofLog() << i.obj->name << " ; parent's translatedPosition = " << i.obj->parent->getTranslatedPosition() << " ; parent's transformedPosition = " << i.obj->parent->transformedPosition;
            i->obj->position = i->obj->parent->touchPoint - i->dragOffset - i->dragOffset2;
            
            if (i->obj->data["strictinside"].asInt() == 1) {
                // STRICT
                if (i->obj->position.x < (0 + i->obj->data["snaptoparent"].asInt() ))
                    i->obj->position.x = 0;
                if (i->obj->position.y < (0 + i->obj->data["snaptoparent"].asInt()))
                    i->obj->position.y = 0;
                
                if (i->obj->parent->size.x < (i->obj->size.x + i->obj->position.x + i->obj->data["snaptoparent"].asInt()))
                    i->obj->position.x = i->obj->parent->size.x - i->obj->size.x;
                
                if (i->obj->parent->size.y < (i->obj->size.y + i->obj->position.y + i->obj->data["snaptoparent"].asInt()))
                    i->obj->position.y = i->obj->parent->size.y - i->obj->size.y;
                
//                ofLog() << "snap to parent = " << i.obj->data["snaptoparent"].asInt();
            }
        }
        if ((!parent->touchingNow) && (i->draggingMe)) {
            ofLog() << "::::CHANGING TO NO DRAG - " << i->obj->name;
            i->dragEndCallback(i->obj);
            color = ofPoint(200, 0, 0);
            
            ofLog() << "::::CALLING SPACER'S TOUCH UP FUNCS";
            i->obj->touchUp();
            i->obj->touchUpC(i->obj);
            i->obj->executeCallbacks(UIObject::TOUCH_UP, i->obj);
            
            i->draggingMe = false;
            i->dragCandidate = false;
            /*
            auto zz = std::find(topParent->focused.begin(), topParent->focused.end(), i->obj);
            if (zz != topParent->focused.end())
                topParent->focused.erase(std::remove(topParent->focused.begin(), topParent->focused.end(), i->obj), topParent->focused.end());*/
        }
        if (!topParent->touchingNow)
            i->dragCandidate = false;
    };
    
    vector<dragData*> objects;
};

#endif
