#ifndef UIShape_h
#define UIShape_h

#include "UIAnimation.h"
#include "ofMain.h"

namespace UIGraph {

class UIShape {
public:
    UIShape() {}
    
    // Override this if you want your object to have a more
    // complicated shape
    virtual bool pointInclusionTest(ofPoint point) {
        return ((point.x > position.x) &&
                (point.x < (position.x + size.x)) &&
                (point.y > position.y) &&
                (point.y < (position.y + size.y)));
    }

    
    Animatable<ofPoint> position, size;
};
    
}
#endif /* UIShape_h */
