

// UIObject.h is a micro UI library.
// (c) Lyosha Zebra, 2015


#ifndef UIObject_h
#define UIObject_h

///
#define DEFAULT_EASING_FACTOR 5
///////////////////////

#include "ofMain.h"
#include <functional>
#include "UIBaseGestureRecognizer.h"
#include "UIFingerManager.h"
#include "UIShape.h"
#include "UIConstraintSolver.h"
#include "LiquidEvent.h"
#include "FunctionStack.h"

class UIObject: public UIFingerManager, public UIShape, public UIConstraintSolver {
public:
    
    ///         Callback system
    
    struct DrawEventArgs {
        UIObject *renderedObject;
        ofPoint position, size;
        
        void sayHi() {
//            ofLog() << "hi from struct's function!";
        };
    };
    
    FunctionStack<void(UIObject*), void, UIObject*> draw;

    FunctionStack<void(UIObject*), void, UIObject*> drawOverlay;

    FunctionStack<void(UIObject*), void, UIObject*> update;
    
    FunctionStack<bool(), bool> touchDown;
    FunctionStack<void(), void> mouseMove;

    FunctionStack<bool(), bool> touchUp;
    
    FunctionStack<bool(UIObject*), bool, UIObject*> touchDragC;
    
    FunctionStack<bool(UIObject*), bool, UIObject*> touchDownC;

    FunctionStack<bool(UIObject*), bool, UIObject*> touchUpC;

    FunctionStack<void(UIObject*), void, UIObject*> mouseMoveC;

    
    ///         Common callbacks
    
    enum EventType {
        TOUCH_UP, TOUCH_DOWN, TOUCH_DRAG, MOUSE_MOVE, UPDATE, DRAW
    };
    
    std::map<EventType, LiquidEvent<void>> commonCallbacks;
    
    void addCallback(EventType ccase, function<void()> callback) {
        commonCallbacks[ccase] += callback;
    };
    
    void executeCallbacks(EventType ccase, UIObject *o) {
        commonCallbacks[ccase].notifyListeners();
    };
 
    std::map <EventType, double> eventStack;
    std::map <string, double> customEventStack;
    
    void registerEvent(EventType type, UIObject *o) {
        eventStack[type] = ofGetElapsedTimeMillis();
        executeCallbacks(type, o);
    }
    
    float timeFromEvent(string type) {
        return ofGetElapsedTimeMillis() - customEventStack[type];
    };
    
    float timeFromEvent(EventType type) {
        return ofGetElapsedTimeMillis() - eventStack[type];
    };
    
    float timeQFromEvent(EventType type, int limitInMilliseconds) {
        if (eventStack.find(type) == eventStack.end())
            return -1; // no such event. returning 1 is bad,
        // should make up some err message instead or smthn
        else return ofClamp(((timeFromEvent(type))/(float)limitInMilliseconds), 0, 1);
    }
    
    ofPoint getTransformedPosition() { // actually it's onscreen position.
                                       // TODO: implement (inside shape, maybe?)
        return transformedPosition;
    }
    
    ofPointF innerTransform;
    
    ///         Constraints
    
    void removeSizeConstraints() {
        // TODO: implement this in ConstraintSolver
    }
    
    void removePositionConstraints() {
        // TODO: implement this in ConstraintSolver
    }
    
    void setSize(float x, float y) {
        removeSizeConstraints();
        size.x = x;
        size.y = y;
    }

    void setPosition(float x, float y) {
        removePositionConstraints();
        position.x = x;
        position.y = y;
    }
    
    void setPosition(UIConstraint X, UIConstraint Y) {
        removePositionConstraints();
        addPositionConstraint(this, UIConstraint2D(X, Y));
    }

    void setSize(UIConstraint X, UIConstraint Y) {
        removeSizeConstraints();
        addSizeConstraint(this, UIConstraint2D(X, Y));
    }
    
    void setSize(float x, UIConstraint Y) {
        removeSizeConstraints();
        addSizeConstraint(this, UIConstraint2D(x, Y));
    }

    void setSize(UIConstraint X, float y) {
        removeSizeConstraints();
        addSizeConstraint(this, UIConstraint2D(X, y));
    }
    
    UIConstraint getConstraintLeft(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::LEFT, p, this, arg);
    }

    UIConstraint getConstraintRight(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::RIGHT, p, this, arg);
    }
    
    UIConstraint getConstraintTop(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::TOP, p, this, arg);
    }

    UIConstraint getConstraintBottom(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::BOTTOM, p, this, arg);
    }

    UIConstraint getConstraintInsideLeft(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::INSIDE_LEFT, p, this, arg);
    }

    UIConstraint getConstraintInsideTop(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::INSIDE_TOP, p, this, arg);
    }
 
    UIConstraint getConstraintInsideRight(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::INSIDE_RIGHT, p, this, arg);
    }
    
    UIConstraint getConstraintInsideBottom(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::INSIDE_BOTTOM, p, this, arg);
    }

    UIConstraint getConstraintCenterOfWidth(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::CENTER_OF_WIDTH, p, this, arg);
    }

    UIConstraint getConstraintCenterOfHeight(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::CENTER_OF_HEIGHT, p, this, arg);
    }

    UIConstraint getConstraintParallelToTop(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::PARALLEL_TO_TOP, p, this, arg);
    }
    
    UIConstraint getConstraintParallelToLeft(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::PARALLEL_TO_LEFT, p, this, arg);
    }

    UIConstraint getConstraintPercentageOfSizeX(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::PERCENTAGE_OF_SIZE_X, p, this, arg);
    }
    
    UIConstraint getConstraintPercentageOfSizeY(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::PERCENTAGE_OF_SIZE_Y, p, this, arg);
    }
    
    UIConstraint getConstraintSizeXMinusArgument(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::SIZE_X_MINUS_ARGUMENT, p, this, arg);
    }
    
    UIConstraint getConstraintSizeYMinusArgument(UIShape *p, float arg) {
        return UIConstraint(UIConstraint::SIZE_Y_MINUS_ARGUMENT, p, this, arg);
    }
    
    ///         Hierarchy management
    
    UIObject* addChild(UIObject *object);
    
    UIObject* getChild(string iD);
    
    bool bypassClippingTest;
    
    // TODO: rename clipping test (it's really a shape's point inclusion test)
    bool clippingTest(UIShape *i);

    ofFbo getFbo() { return myFbo;}
    ofFbo* getFboPointer() { return &myFbo;}
    
    ofFbo myFbo;
    bool useFbo;
    
    void setFboSize(ofPoint size) {
//        ofLog() << "myFbo size x = " << myFbo.getWidth() << " ; required = " << size.x;
        myFbo.allocate(size.x, size.y);
    };
    
    void getToTheFront() {
        zIndex = parent->getMaxIndex() + 1;
    }
    
    int getMaxIndex() {
        int current_zIndex = 0, maxZIndex = zIndex;
        for (auto i: children) {
            //if (i->zIndex > 10) i->zIndex = 5;
            if (maxZIndex < i->zIndex) {
                maxZIndex = i->zIndex;
            }
        }
        return maxZIndex;
    };
    
    
    UIBaseGestureRecognizer* gestureRecognizer; // should be vector of recognizers instead
    
    static std::map<int, ofPoint> fingerPositions;
    static std::map<int, bool> fingerIsDown;
    
    // TODO: move this to CPP
    bool touchBroadcast(ofPoint touchPosition, EventType type, int fingerIndex = 0, int level = 0) {
        if (!visible) return false;
        
        int counter = 0;
        int current_zIndex = 0, maxZIndex = zIndex;
        for (auto i: children) {
            if (maxZIndex < i->zIndex) {
                    maxZIndex = i->zIndex;
            }
            if (maxZIndex < i->getMaxIndex()) {
                maxZIndex = i->getMaxIndex();
            }
            counter++;
        }
        current_zIndex = maxZIndex;
        
        if (level == 0) {
            fingerPositions[fingerIndex] = touchPosition;
        }
        if (type == TOUCH_UP) {
            fingerIsDown[fingerIndex] = false;
        }
        if (type == TOUCH_DOWN) {
            fingerIsDown[fingerIndex] = true;
        }
        
        if (isFingerCaptured(fingerIndex)) {
            UIBaseGestureRecognizer* gs = ((UIBaseGestureRecognizer*)getRecognizerForFinger(fingerIndex));
            if (type == TOUCH_DOWN) {
                gs->touchDown(touchPosition, fingerIndex);
            }
            if (type == TOUCH_DRAG) {
                gs->touchDrag(touchPosition, fingerIndex);
            }
            if (type == TOUCH_UP) {
                gs->touchUp(touchPosition, fingerIndex);
            }
            return true;
        }
        
        bool result = false;
        while ((!result) && (current_zIndex >= 0) ) {
            for (auto i: children) {
                
                
                if ( (i->zIndex == current_zIndex) &&
                    (i->pointInclusionTest(touchPosition - innerTransform) &&
                     (i->visible))
                    ) {
                        i->lastTouched = ofGetElapsedTimeMillis();
                        i->lastTouchedPosition = touchPosition - i->position - innerTransform;
                        // ^ TODO: get rid of that. Streamline pointer operations
                        i->transformedPosition = transformedPosition + i->position;
                    
                         if (type == TOUCH_DOWN) {
//                            ofLog() << "touched DOWN " << i->name << " with zIndex = "<< current_zIndex;
                             
                             bool gestureRecognizerTookOver = false;
                             if (i->gestureRecognizer != NULL) {
                                 i->gestureRecognizer->offset = fingerPositions[fingerIndex] - (touchPosition - i->position);
                                 gestureRecognizerTookOver = i->gestureRecognizer->touchDown(touchPosition - i->position, fingerIndex);
                             }
                             
                             if (gestureRecognizerTookOver)
                                 return true;
                             
                             if ((!i->touchBroadcast(touchPosition - i->position, type, fingerIndex, level + 1)) ||
                                  (i->children.size() == 0)) {
                                 
                                if ((i->touchDown()) || (i->touchDownC(i))) {
                                    i->registerEvent(TOUCH_DOWN, i);
                                    return true;
                                }
                                 
                             } else {
                                return true;
                             }
                        }

                        if (type == TOUCH_DRAG) {
                            if (i->gestureRecognizer != NULL) {
                                i->gestureRecognizer->offset = fingerPositions[fingerIndex] - (touchPosition - i->position);
                                i->gestureRecognizer->touchDrag(touchPosition - i->position, fingerIndex);
                            }
                            
                            if (!i->touchBroadcast(touchPosition - i->position, type, fingerIndex, level + 1)) {
                                if ((i->touchDragC(i))) {
                                    i->registerEvent(TOUCH_DRAG, i);
                                    return true;
                                }
                            } else {
                                return true;
                            }
                        }
                    
                        if (type == MOUSE_MOVE) {
                            
                            if (i->gestureRecognizer != NULL) {
                                // the following is not implemented in gesture rec just yet
                                i->gestureRecognizer->mouseMove(touchPosition - i->position);
                            }

                            
                            if (!i->touchBroadcast(touchPosition - i->position, type, fingerIndex, level + 1)) {
                                i->mouseMove();
                                i->mouseMoveC(i);
                                i->registerEvent(MOUSE_MOVE, i);
                                i->executeCallbacks(UIObject::MOUSE_MOVE, i);
                                return true;
                            } else {
                                //ofLog() << "touch move ended up inside children of " << i->name;
                                return true;
                            }

                        }
                    
                        if (type == TOUCH_UP) {
                            if (i->gestureRecognizer != NULL) {
                                i->gestureRecognizer->touchUp(touchPosition - i->position, fingerIndex);
                                i->gestureRecognizer->offset = fingerPositions[fingerIndex] - (touchPosition - i->position);
                            }

                            if (!i->touchBroadcast(touchPosition - i->position, type, fingerIndex, level + 1)) {
                                if ((i->touchUp()) || (i->touchUpC(i))) {
                                    i->registerEvent(TOUCH_UP, i);

                                    return true;
                                }
                            } else {
                                return true;
                            }
                        }
                } else {
                }
            }
            current_zIndex --;
        }
        return false;
    }

    ///                Convenience
    
    operator UIShape* () {
        return (UIShape*)this;
    }
    
    ///                Constructors

    
    UIObject () {
		init();
    }

    UIObject (ofPoint pos, ofPoint siz) {
		init();
        position = pos;
        size = siz;
    }

    UIObject (string nam) {
        init();
        name = nam;
    }

    
    UIObject (string nam, ofPoint pos, ofPoint siz) {
		init();
        position = pos;
        size = siz;
        name = nam;
    }

    UIObject (string nam, ofPoint pos, ofPoint siz, int z_index) {
		init();
        position = pos;
        size = siz;
        name = nam;
        zIndex = z_index;
    }

    UIObject (string nam, ofPoint pos, ofPoint siz, bool fbo) {
        init();
        position = pos;
        size = siz;
        name = nam;
        // WARNING!! Test case
        // TODO: test this
        if (fbo)
            myFbo.allocate(ofGetWidth(), ofGetHeight());
        useFbo = fbo;
    }

    UIObject (ofPoint pos, ofPoint siz, bool fbo) {
        init();
        position = pos;
        size = siz;
        // WARNING!! Test case
        // TODO: test this
        if (fbo)
            myFbo.allocate(ofGetWidth(), ofGetHeight());
        useFbo = fbo;
    }

    UIObject (string nam, ofPoint pos, ofPoint siz, int z_index, bool fbo) {
        init();
        position = pos;
        size = siz;
        name = nam;
        zIndex = z_index;
        // WARNING!! Test case
        // TODO: test this
        if (fbo)
            myFbo.allocate(ofGetWidth(), ofGetHeight());
        useFbo = fbo;
    }
    
    UIObject (string nam, float x, float y, float w, float h) {
        init();
        position = ofPoint(x, y);
        size = ofPoint(w, h);
        name = nam;
        
    }
    
    // TODO: some more constructors
    
    bool clipTestChildren; // clipTest checks roughly if the object's children is on screen not to
                   // render too much garbage
    bool clipTestMe;
    
//    UIObject *topParent;
    UIObject* getTopParent() {
        UIObject *attempt = this;
        while (attempt->parent != attempt) {
            attempt = attempt->parent;
        }
        return attempt;
    }
    string report() {
        return "object " + name + " with parent " + parent->name + " and zIndex = " + ofToString(zIndex);
    }
    
    UIObject *parent;
    std::vector<UIObject*> children;
    long lastTouched;
    ofPointF lastTouchedPosition; // <-- probably unused
    string name;
    
    bool visible;
    
    bool useViewport;
    
    int zIndex;
    
    ofPointF color;
    
protected:
    /*    Application lifecycle (call this from canvas class)           */
    
        void updateCycle(bool updateAnimator = true);
    
        void drawCycle();

    void init() {
        color = ofPoint(ofRandom(255), ofRandom(255), ofRandom(255));
        parent = this;
        
        draw = [&](UIObject *obj){};
        
        update = [&](UIObject *obj){};
        
        touchDown = []()->bool {
            return false;
        };
        
        mouseMove = []()->bool {
            return false;
        };
        
        touchUp = []()->bool {
            return false;
        };
        
        touchDownC = [](UIObject *o)->bool {
            return false;
        };
        
        touchUpC = [](UIObject *o)->bool {
            return false;
        };
        
        mouseMoveC = [](UIObject *o)->bool {
            return false;
        };
        
        touchDragC = [&](UIObject *o)->bool {
            return false;
        };
        
        clipTestChildren = true;
        clipTestMe = true;
        zIndex = 0;
        visible = true;
        useFbo = false;
        useViewport = false;
        
        gestureRecognizer = NULL;
    }

private:
    ofPoint transformedPosition;

};

#endif
