
#ifndef UIAnimation_h
#define UIAnimation_h

#include "ofMain.h"

namespace UIGraph {

class UIAnimation {
public:
    static int count;
    
    static void update() {
        timestamp = ofGetElapsedTimef();
    }
    
    double getTimestamp() {
        return timestamp;
    }
    
private:
    static double timestamp;

};

template <typename T>
class Animatable: public UIAnimation, public T {
public:
    // Base
    Animatable() {
        inner = T();
        timeStarted = ofGetElapsedTimef(); // TODO: replace it with static UIAnimation's timestamp
        animationTime = 0;
    }
    
//    operator T & () {
//        animate();
//        return inner;
//    }
//
    T& operator=(const T & v) {
        // if you've started writing animation code, i.e.
        //
        // Animatables::listenToYourNewAnimationObjectives()
        //      Animatable1::moveTo(100, 200);
        //      Animatable2::moveTo(200, 400);
        // Animatables::stopListening()
        //
        // then this operator is a place to check for static var if it's
        // started and maybe put v into the target and not the inner,
        // accordingly
		inner = v;
        T::operator=(v);
        return *this;
	}

    // Animation
    float easing(float arg) {
        return (
                pow(arg, 3) / (pow(arg, 3) + pow(1 - arg, 3))
                );
    }
    
    bool isAnimating() {
        if (timeStarted > 2) {
            // TODO: Sometimes something animates without a reason.
            //  ofLog() << "animating:" << ((ofGetElapsedTimef() - timeStarted) - animationTime);
        }
        return ((ofGetElapsedTimef() - timeStarted) < animationTime);
    };
    
    // This isn't quite there yet.
    // The animation is need to be called from the outside,
    // from the static UIAnimation class
    void animate() {
        
        if (isAnimating()) {
            float phase = easing(((ofGetElapsedTimef() - timeStarted) / animationTime));
            inner = target * phase + previous * (1 - phase);
            T::operator=(inner);
            //this = target;
        }
        
        T::operator=(inner);
    }
    
    void animateTo(T target_, float time = 1) {
        previous = inner;
        target = target_;
        animationTime = time;
        timeStarted = ofGetElapsedTimef();
    }
    
    
//protected:
    T inner; // <-- this is not needed anymore, abandon it
    T target;
    T previous;
    double animationTime, timeStarted;
};

typedef Animatable<ofPoint> ofPointF;
typedef Animatable<ofColor> ofColorF;

}

#endif
