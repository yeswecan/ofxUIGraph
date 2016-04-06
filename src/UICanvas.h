//
//  UICanvas.h
//

#ifndef UICanvas_h
#define UICanvas_h

#include "UIObject.h"
#include "ofMain.h"
#include "UIKeyboardEventReciever.h"

namespace UIGraph {
class UICanvas: public UIObject {
public:
    UICanvas() {
        ofRegisterKeyEvents(this);
        ofRegisterMouseEvents(this);
        ofAddListener(ofEvents().windowResized, this, &UICanvas::windowResized);
        ofRegisterTouchEvents(this);
        
        name = "canvas";
        
        parent = this;
    }
    
    bool initialized = false;
    
    
    void update() {
        if (!initialized) {
            size = ofPoint(ofGetWidth(), ofGetHeight());
            initialized = true;
        }
        solveConstraints();
        UIAnimation::update();
        updateCycle();
    }
    
    void draw() {
        ofSetColor(255);
        drawCycle();
    }
    
    bool touchBroadcast(ofPoint touchPosition, EventType type, int fingerIndex = 0, int level = 0) {
        bool result = UIObject::touchBroadcast(touchPosition, type, fingerIndex, level);
        registerEvent(type, this);
		return result;
    }
    
    // Window events
    
    void windowResized(ofResizeEventArgs & args) {
        size = ofPoint(args.width, args.height);
    }
    
    // Keyboard events
    
    void keyPressed(ofKeyEventArgs & args) {
        UIKeyboardEventReciever::broadcastKeyPressed(args.key);
    }

    void keyReleased(ofKeyEventArgs & args) {
        UIKeyboardEventReciever::broadcastKeyReleased(args.key);
    }
    
    // Mouse events
    
    void mouseDragged(ofMouseEventArgs & args) {
        touchBroadcast(args, UIObject::TOUCH_DRAG);
    }

    void mouseMoved(ofMouseEventArgs & args) {
        touchBroadcast(args, UIObject::MOUSE_MOVE);
    }

    void mousePressed(ofMouseEventArgs & args) {
        touchBroadcast(args, UIObject::TOUCH_DOWN);
    }

    void mouseReleased(ofMouseEventArgs & args) {
        touchBroadcast(args, UIObject::TOUCH_UP);
    }

    void mouseScrolled(ofMouseEventArgs & args) {
        // ?
    }

    void mouseEntered(ofMouseEventArgs & args) {
        // ?
    }

    void mouseExited(ofMouseEventArgs & args) {
        // ?
    }
    
    // Touch events
    
    void touchDown(ofTouchEventArgs & args) {
        touchBroadcast(args, UIObject::TOUCH_DOWN);
    }
    
    void touchMoved(ofTouchEventArgs & args) {
        touchBroadcast(args, UIObject::TOUCH_DRAG);
    }

    void touchUp(ofTouchEventArgs & args) {
        touchBroadcast(args, UIObject::TOUCH_UP);
    }
    
    void touchCancelled(ofTouchEventArgs & args) {
        // TODO: something better
        touchBroadcast(args, UIObject::TOUCH_UP);
    }
    
    void touchDoubleTap(ofTouchEventArgs & args) {
        // ?
    }
};
}

#endif /* UICanvas_h */
