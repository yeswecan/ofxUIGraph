

// UIObject.h is a micro UI library.
// (c) Lyosha Zebra, 2015


#ifndef UIObject_h
#define UIObject_h

///
#define DEFAULT_EASING_FACTOR 5
///////////////////////

#include "ofxJSONElement.h"
#include "ofxXMLSettings.h"
#include "ofMain.h"
#include <functional>
#include "UIAnimation.h"
#include "UIGestureRecognizer.h"
#include "UIGestureRecognizerServer.h"

class UIObject: public UIGestureRecognizerServer {
public:
    //////// ***********  CALLBACKS  ***********  //////////
    // TODO: a vector of generic callbacks with a list of possible
    // events to react to.
    
    /*           Replace this with your own lambdas            */
    
    function<void(UIObject*)> draw;

    function<void(UIObject*)> update;
    
    function<void()> drawWithoutContext;
    
    function<void()> updateWithoutContext;
    
    function<bool()> touchDown;
    function<bool()> touchMove;

    function<bool()> touchUp;
    
    function<bool(UIObject*)> touchDownC;

    function<bool(UIObject*)> touchUpC;

    function<bool(UIObject*)> touchMoveC;

    
    /*              Common callbacks                 */
    
    enum EventType {
        TOUCH_UP, TOUCH_DOWN, TOUCH_DRAG, MOUSE_MOVE, UPDATE, DRAW
    };
    // TODO: add additional callback struct with parameters,
    // e.g. touch point, etc
    struct commonCallback {
        function<void(UIObject*)> callback;
        EventType ccase;
        string cid;
    };
    vector <commonCallback> commonCallbacks;
    
    void addCallback(EventType ccase, function<void(UIObject*)> callback) {
        commonCallback addon;
        addon.ccase = ccase; addon.callback = callback;
        commonCallbacks.push_back(addon);
    };
    
    void addCallback(EventType ccase, function<void(UIObject*)> callback, string cid) {
        commonCallback addon;
        addon.ccase = ccase; addon.callback = callback;
        addon.cid = cid;
        commonCallbacks.push_back(addon);
    };
    
    void executeCallbacks(EventType ccase, UIObject *o) {
        registerEvent(ccase);
        for (auto i: commonCallbacks) {
            if (i.ccase == ccase)
                i.callback(o);
        }
    };
    
    /*                      Class routines                     */
    
    UIObject* addChild(UIObject *object);
    
    UIObject* getChild(string iD);
    
    void addXML(ofxXmlSettings *addition, UIObject *targetParent);
    
    void parseXML(string filename);
    
    void parseJSON(string filename);
    
    /*    Application work cycle (call this from OF)           */

    void updateCycle(bool updateAnimator = true);
    
    void drawCycle();
    
    bool bypassClippingTest;
    
    // TODO: clipping test is too simple and not accurate in most
    // situations
    
    bool clippingTest(UIObject *i);

    
    ofFbo getFbo() { return myFbo;}
    ofFbo* getFboPointer() { return &myFbo;}
    
    ofFbo myFbo;
    bool useFbo;
    
    void setFboSize(ofPoint size) {
        // TODO: free resources first!
        myFbo.allocate(size.x, size.y);
    };
    
    bool innerViewport;
    ofPoint innerViewportSize;
    
//    enum TouchType {DOWN, MOVE, DRAG, UP};
    
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
    
    bool pointInclusionTest(UIObject *i, ofPoint point) {
        ofRectangle rect = ofRectangle(i->position.x, i->position.y, i->size.x, i->size.y);
        return ((point.x > i->position.x) &&
                (point.x < (i->position.x + i->size.x)) &&
                (point.y > i->position.y) &&
                (point.y < (i->position.y + i->size.y)));
    }
    
    
    bool touchingNow;
    ofPoint touchPoint;
    
    static std::map<int, ofPoint> fingerPositions;
    static std::map<int, bool> fingerIsDown;
    
    UIGestureRecognizer* gestureRecognizer = NULL; // should be vector of recognizers instead
    
    bool touchBroadcast(ofPoint touchPosition, EventType type, int fingerIndex = 0, int level = 0) {
        if (!visible) return false;
        
        int counter = 0;
        int current_zIndex = 0, maxZIndex = zIndex;
        for (auto i: children) {
            //ofLog() << counter << ":" << i->name << " - " << i->zIndex;
            //if (i->zIndex > 10) i->zIndex = 5;
            if (maxZIndex < i->zIndex) {
                    maxZIndex = i->zIndex;
            }
            if (maxZIndex < i->getMaxIndex()) {
                maxZIndex = i->getMaxIndex();
            }
            counter++;
        }
        current_zIndex = maxZIndex;
        
        touchPoint = touchPosition; // <- this is uneffective
        
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
            // Gesture recognizer has this!!
            // need to call it now instead
            UIGestureRecognizer* gs = ((UIGestureRecognizer*)getRecognizerForFinger(fingerIndex));
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
                    (pointInclusionTest(i, touchPosition - innerTransform) &&
                     (i->visible))
                    ) {
                        i->lastTouched = ofGetElapsedTimeMillis();
                        i->lastTouchedPosition = touchPosition - i->position - innerTransform;
                        // TODO: ^^ make it a per finger thing inside every object maybe?
                        // could be useful for touch gesture recognizers
                    
                         if (type == TOUCH_DOWN) {
                            ofLog() << "touched DOWN " << i->name << " with zIndex = "<< current_zIndex;
                             
                             if (i->gestureRecognizer != NULL) {
                                 i->gestureRecognizer->offset = fingerPositions[fingerIndex] - (touchPosition - i->position);
                                 i->gestureRecognizer->touchDown(touchPosition - i->position, fingerIndex);
                             }
                             
                             if ((!i->touchBroadcast(touchPosition - i->position, type, fingerIndex, level + 1)) ||
                                  (i->children.size() == 0)) {
                                ofLog() << "touchbroadcast = false";
                                if ((i->touchDown()) || (i->touchDownC(i))) {
                                    ofLog() << " --- touch down ended up inside " << i->name;
                                    i->registerEvent(TOUCH_DOWN);
                                    i->executeCallbacks(UIObject::TOUCH_DOWN, i);
                                    return true;
                                }
                             } else {
                                ofLog() << "touch down ended up inside children of " << i->name;
                                ofLog() << i->name << " has " << i->children.size() << " children";
                                return true;
                             }
                        }

                        if (type == TOUCH_DRAG) {
//                            ofLog() << "touched DRAG " << i->name << " with zIndex = "<< current_zIndex;
                            
                            if (i->gestureRecognizer != NULL) {
                                i->gestureRecognizer->offset = fingerPositions[fingerIndex] - (touchPosition - i->position);
                                i->gestureRecognizer->touchDrag(touchPosition - i->position, fingerIndex);
                            }
                            
                            if (!i->touchBroadcast(touchPosition - i->position, type, fingerIndex, level + 1)) {
//                                ofLog() << "touchbroadcast = false";
//                                if ((i->touchDown()) || (i->touchDownC(i))) {
      //                              ofLog() << " --- touch down ended up inside " << i->name;
                                    i->registerEvent(TOUCH_DRAG);
                                    i->executeCallbacks(UIObject::TOUCH_DRAG, i);
    //                                return true;
  //                              }
                            } else {
                                ofLog() << "touch down ended up inside children of " << i->name;
                                return true;
                            }
                        }
                    
                        if (type == MOUSE_MOVE) {
                            
                            if (i->gestureRecognizer != NULL) {
                                // the following is not implemented in gesture rec
                                //i->gestureRecognizer->touchMove(touchPosition - i->position, fingerIndex);
                            }

                            
                            if (!i->touchBroadcast(touchPosition - i->position, type, fingerIndex, level + 1)) {
                                if ((i->touchMove()) || (i->touchMoveC(i))) {
                                    i->registerEvent(TOUCH_DRAG);
                                    i->executeCallbacks(UIObject::TOUCH_DRAG, i);
                                    i->executeCallbacks(UIObject::MOUSE_MOVE, i);
                                    //ofLog() << " --- touch move ended up inside " << i->name;
                                    return true;
                                }
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
                                    i->registerEvent(TOUCH_UP);
                                    i->executeCallbacks(UIObject::TOUCH_UP, i);

                                    ofLog() << " --- touch up ended up inside " << i->name;
                                    return true;
                                }
                            } else {
                                ofLog() << "touch move ended up inside children of " << i->name;
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
    
    
    /*                  Helpers               */
    std::map <EventType, double> eventStack;
    std::map <string, double> customEventStack;

    void registerEvent(string type) {
        customEventStack[type] = ofGetElapsedTimeMillis();
    }

    void registerEvent(EventType type) {
        eventStack[type] = ofGetElapsedTimeMillis();
    }

    float timeFromEvent(string type) {
        return ofGetElapsedTimeMillis() - customEventStack[type];
    };
    
    float timeFromEvent(EventType type) {
        return ofGetElapsedTimeMillis() - eventStack[type];
    };
    
    float timeQFromEvent(EventType type, int limitInMilliseconds) {
        if (eventStack.find(type) == eventStack.end())
            return 1; // no such event. returning 1 is bad,
                      // should make up some err message instead or smthn
        else return ofClamp(((timeFromEvent(type))/(float)limitInMilliseconds), 0, 1);
    }
    
    // TODO: timeQFromLastTouch and lastTouched should contain
    // and return all kinds of info FROM MULTIPLE SOURCES,
    // e.g. different fingers, mouse, whatever
    double timeQFromLastTouch(int limitInMilliseconds) {
        if (lastTouched > ofGetElapsedTimeMillis())
            return 1;
        if ((ofGetElapsedTimeMillis() - lastTouched) > limitInMilliseconds) return 1;
            else return ((double)(ofGetElapsedTimeMillis() - lastTouched) / (double)limitInMilliseconds);
    };
    
    int millisFromLastTouch() {
        return (ofGetElapsedTimeMillis() - lastTouched);
    }
    
    ofPoint transformedPosition;
    ofPointF innerTransform;
    
    /*                Constructors            */

	void init() {
        color = ofPoint(ofRandom(255), ofRandom(255), ofRandom(255));
        topParent = this;
        parent = this;

    	draw = [&](UIObject *obj){};

		update = [&](UIObject *obj){};
    
		drawWithoutContext = [&](){};
    
		updateWithoutContext = []() {};
    
		touchDown = []()->bool {
			return false;
		};
		touchMove = []()->bool {
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

		touchMoveC = [](UIObject *o)->bool {
			return false;
		};

		clipTestChildren = true;
		clipTestMe = true;
		zIndex = 0;
        visible = true;
		useFbo = false;
	}
    
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
        ofLog() << "created object " << nam << " with zindex = " << zIndex;
    }

    UIObject (string nam, ofPoint pos, ofPoint siz, bool fbo) {
        position = pos;
        size = siz;
        name = nam;
        if (fbo)
            myFbo.allocate(ofGetWidth(), ofGetHeight());
        useFbo = fbo;
        //ofLog() << "CALLED CONSTRUCTOR FOR " << nam << (fbo ?  " to use FBO" : " NOT TO USE FBO");
    }
    
    UIObject (string nam, ofPoint pos, ofPoint siz, int z_index, bool fbo) {
        position = pos;
        size = siz;
        name = nam;
        zIndex = z_index;
        // WARNING!! Test case
        if (fbo)
            myFbo.allocate(ofGetWidth(), ofGetHeight());
        useFbo = fbo;
    }
    
    // TODO: some more constructors
    
    bool clipTestChildren; // clipTest checks roughly if the object's children is on screen not to
                   // render too much garbage
    bool clipTestMe;
    
    UIObject *topParent;
    UIObject* getTopParent() {return topParent;}
    string report() {
        return "object " + name + " with parent " + parent->name + " and zIndex = " + ofToString(zIndex);
    }
    
    UIObject *parent;
    vector<UIObject*> children;
    ofxJSONElement data;
    long lastTouched;
    ofPointF lastTouchedPosition; // it's used in DragDispatcher and it's a shame.
    // Need to do something about it.
    string name;
    
    bool visible;
    
    bool useViewport = false;
    
    int zIndex;
    
    ofPointF position, size;
    ofPointF color;
    
};

#endif
