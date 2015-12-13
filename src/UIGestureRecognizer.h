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
    UIGestureRecognizer(){
	    gestureStarted = [](UIGestureRecognizer* g) {};
	    gestureUpdated = [](UIGestureRecognizer* g) {};
		gestureEnded = [](UIGestureRecognizer* g) {};

	}
    
    
    // Return true if UIObject shouldn't broadcast further
    virtual bool touchDown(ofPoint point, int finger) {
		return true;        
    };

    // The same as above
    virtual bool touchDrag(ofPoint point, int finger) {
		return true;        
    };
    
    // The same too
    virtual bool touchUp(ofPoint point, int finger) {
		return true;        
    };
    
    virtual void mouseMove(ofPoint point) {
        
    }

    /// State
    
    function<void(UIGestureRecognizer*)> gestureStarted;
    function<void(UIGestureRecognizer*)> gestureUpdated;
    function<void(UIGestureRecognizer*)> gestureEnded;
  
    ofPoint offset;
};

#endif /* UIGestureRecognizer_hpp */
