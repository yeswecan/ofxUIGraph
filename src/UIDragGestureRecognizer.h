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

class UIDragGestureRecognizer: public UIGestureRecognizer {
public:
    UIDragGestureRecognizer() {
        dragStarted = false;
    }
    
    // Return true if UIObject shouldn't broadcast further
    bool touchDown(ofPoint point, int finger) {
        dragFinger = finger;
    };
    
    // The same as above
    bool touchDrag(ofPoint point, int finger) {
        if (!dragStarted) {
            UIGestureRecognizerServer::captureFinger(finger, this);
            gestureStarted(this);
            dragStarted = true;
            
            dragOffset = UIObject::fingerPositions[finger] - offset;
        }
        
//        ofLog() << "drag update!";
        gestureUpdated(this);
    };
    
    // The same too
    bool touchUp(ofPoint point, int finger) {
        if (dragStarted) {
            UIGestureRecognizerServer::releaseFinger(finger);
            gestureEnded(this);
            dragStarted = false;
        }
    };

    int dragFinger;
    
    bool dragStarted;
    
    ofPoint dragOffset;
};

#endif /* UIDragGestureRecognizer_h */
