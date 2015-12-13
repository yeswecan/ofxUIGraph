//
//  UIGestureRecognizerServer.h
//  uiGraphTest
//
//  Created by zebra on 04.11.15.
//
//

#ifndef UIGestureRecognizerServer_h
#define UIGestureRecognizerServer_h

#include "UIGestureDataReciever.h"
#include "UIGestureRecognizer.h"
#include "ofMain.h"


class UIGestureRecognizerServer {
public:
    UIGestureRecognizerServer() {
        for (int i = 0; i < 10; i++) {
            capturedFingers[i] = NULL;
        }
    }
    
    
    static std::map<int, UIGestureDataReciever*> capturedFingers;

    // The following functions are called by gesture recognizers
    static void captureFinger(int finger, UIGestureDataReciever *r) {
        capturedFingers[finger] = r;
    }
    
    static void releaseFinger(int finger) {
        capturedFingers[finger] = NULL;
    };
    
    // The following is called by the child class to check fingers and recognizers
    
    UIGestureDataReciever* getRecognizerForFinger(int finger) {
        return capturedFingers[finger];
    }
    
    bool isFingerCaptured (int finger) {
        return (capturedFingers[finger] != NULL);
    }
};

#endif /* UIGestureRecognizerServer_h */
