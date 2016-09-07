//
//  UIGestureRecognizerServer.h
//  uiGraphTest
//
//  Created by zebra on 04.11.15.
//
//

#ifndef UIGestureRecognizerServer_h
#define UIGestureRecognizerServer_h

#include "UIInputReceiver.h"
#include "UIBaseGestureRecognizer.h"
#include "ofMain.h"


class UIFingerManager {
public:
    UIFingerManager() {
        if (!initialized) {
            for (int i = 0; i < 100; i++) capturedFingers[i] = NULL;
            initialized = true;
        }
    }
    
    bool initialized = false;
    
    static std::map<int, UIInputReceiver*> capturedFingers;

    // The following functions are called by gesture recognizers
    static void captureFinger(int finger, UIInputReceiver *r) {
        capturedFingers[finger] = r;
    }
    
    static void releaseFinger(int finger) {
        capturedFingers[finger] = NULL;
    };
    
    // The following is called by the child class to check fingers and recognizers
    
    UIInputReceiver* getRecognizerForFinger(int finger) {
        return capturedFingers[finger];
    }
    
    bool isFingerCaptured (int finger) {
        return (capturedFingers[finger] != NULL);
    }
};

#endif /* UIGestureRecognizerServer_h */
