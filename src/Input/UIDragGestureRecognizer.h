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
        gestureAbandoned = false;
        lazyMode = false;
    }
    
    struct DragGestureArgs {
        ofPoint offset;
        ofPoint draggedPoint;
        ofPoint initialPoint;
        int dragFinger = 0;
        UIDragGestureRecognizer *recognizer;
    };
    
    UIDragGestureRecognizer(function<void(DragGestureArgs args)> gestureStarted_,
                            function<void(DragGestureArgs args)> gestureUpdated_,
                            function<void(DragGestureArgs args)> gestureEnded_): UIDragGestureRecognizer() {
        gestureStarted = gestureStarted_;
        gestureUpdated = gestureUpdated_;
        gestureEnded = gestureEnded_;
    }
    
    // Return true if UIObject shouldn't broadcast further
    bool touchDown(ofPoint point, int finger) {
        dragFinger = finger;
        gestureAbandoned = false;

        if ((!dragStarted)&&(!gestureAbandoned)) {
            UIFingerManager::captureFinger(finger, this);
            dragStarted = true;
            dragFinger = finger;
            
            dragOffset = UIObject::fingerPositions[finger] - offset;
            initialPoint = point;
            
            gestureStarted(getArgs(point));
            
            lastRegisteredPoint = point;
        }
		return true;
    };
    
    // The same as above
    bool touchDrag(ofPoint point, int finger) {
        if ((!gestureAbandoned) && (finger == dragFinger)) {
            gestureUpdated(getArgs(point));
            
            lastRegisteredPoint = point;
        }
        
		return true;
    };
    
    // The same too
    bool touchUp(ofPoint point, int finger) {
        if ((dragStarted) && (finger == dragFinger)) {
            UIFingerManager::releaseFinger(finger);
            gestureEnded(getArgs(point));
            dragStarted = false;
            
            lastRegisteredPoint = point;
        }
        
		return true;
    };
    
    void stop() {
        if (dragStarted) {
            UIFingerManager::releaseFinger(dragFinger);
            gestureEnded(getArgs(lastRegisteredPoint));
            dragStarted = false;
            gestureAbandoned = true;
        }
    }
    
    
    // Public callbacks
    
    std::function<void(DragGestureArgs args)> gestureStarted;
    std::function<void(DragGestureArgs args)> gestureUpdated;
    std::function<void(DragGestureArgs args)> gestureEnded;

    // Soon to-be-private settings
    
    int dragFinger;
    bool dragStarted, gestureAbandoned, lazyMode;
    ofPoint dragOffset, lastRegisteredPoint, initialPoint;

    
private:
    
    DragGestureArgs getArgs(ofPoint p) {
        DragGestureArgs result;
        result.offset = dragOffset;
        result.draggedPoint = p;
        result.dragFinger = dragFinger;
        result.recognizer = this;
        result.initialPoint = initialPoint;
        
        return result;
    }

};

typedef UIDragGestureRecognizer::DragGestureArgs DragGestureArgs;
    
#endif /* UIDragGestureRecognizer_h */
