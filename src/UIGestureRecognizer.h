//
//  UIGestureRecognizer.hpp
//  uiGraphTest
//
//  Created by zebra on 04.11.15.
//
//

#ifndef UIGestureRecognizer_hpp
#define UIGestureRecognizer_hpp

#include <stdio.h>
#include "ofMain.h"
#include "UIGestureRecognizerServer.h"
#include "UIGestureDataReciever.h"

class UIGestureRecognizer: public UIGestureDataReciever {
public:
    UIGestureRecognizer(){}
    
    
    // Return true if UIObject shouldn't broadcast further
    virtual bool touchDown(ofPoint point, int finger) {
        
    };

    // The same as above
    virtual bool touchDrag(ofPoint point, int finger) {
        
    };
    
    // The same too
    virtual bool touchUp(ofPoint point, int finger) {
        
    };

    /// State
    
    function<void(UIGestureRecognizer*)> gestureStarted = [](UIGestureRecognizer* g) {};
    function<void(UIGestureRecognizer*)> gestureUpdated = [](UIGestureRecognizer* g) {};
    function<void(UIGestureRecognizer*)> gestureEnded = [](UIGestureRecognizer* g) {};
  
    ofPoint offset;
};

#endif /* UIGestureRecognizer_hpp */
