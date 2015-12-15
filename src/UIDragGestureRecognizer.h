//
//  UIDragGestureRecognizer.h
//  uiGraphTest
//
//  Created by zebra on 04.11.15.
//
//

#ifndef UIDragGestureRecognizer_h
#define UIDragGestureRecognizer_h

#include "UIGestureRecognizer.h"

namespace UIGraph {

class UIDragGestureRecognizer: public UIGestureRecognizer {
public:
    UIDragGestureRecognizer() {
        dragStarted = false;
        gestureAbandoned = false;
        lazyMode = false;
    }
    
    UIDragGestureRecognizer(function<void(UIGestureRecognizer*)> gestureStarted_,
                            function<void(UIGestureRecognizer*)> gestureUpdated_,
                            function<void(UIGestureRecognizer*)> gestureEnded_): UIDragGestureRecognizer() {
        gestureStarted = gestureStarted_;
        gestureUpdated = gestureUpdated_;
        gestureEnded = gestureEnded_;
    }
    
    // Return true if UIObject shouldn't broadcast further
    bool touchDown(ofPoint point, int finger) {
        dragFinger = finger;
        gestureAbandoned = false;

        if ((!dragStarted)&&(!gestureAbandoned)) {
            UIGestureRecognizerHost::captureFinger(finger, this);
            dragStarted = true;
            
            dragOffset = UIObject::fingerPositions[finger] - offset;
            gestureStarted(this);
        }
		return true;
    };
    
    // The same as above
    bool touchDrag(ofPoint point, int finger) {
        if (!gestureAbandoned)
            gestureUpdated(this);

		return true;
    };
    
    // The same too
    bool touchUp(ofPoint point, int finger) {
        if (dragStarted) {
            UIGestureRecognizerHost::releaseFinger(finger);
            gestureEnded(this);
            dragStarted = false;
        }
		return true;
    };
    
    void stop() {
        if (dragStarted) {
            UIGestureRecognizerHost::releaseFinger(dragFinger);
            gestureEnded(this);
            dragStarted = false;
            gestureAbandoned = true;
        }
    }

    int dragFinger;
    
    bool dragStarted, gestureAbandoned, lazyMode;
    
    ofPoint dragOffset;
};
    
}

#endif /* UIDragGestureRecognizer_h */
