
#ifndef UIAnimation_h
#define UIAnimation_h

namespace UIGraph {

class UIAnimation {
public:
    static int count;
    
    
};

template <typename T>
class Animatable: public UIAnimation, public T {
public:
    // Base
    Animatable() {
        inner = T();
        timeStarted = ofGetElapsedTimef();
        animationTime = 0;
		ofLog() << "animatable constructor";
    }
    
    operator T & () {
        animate();
        return inner;
    }

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
            ofLog() << "animating:" << ((ofGetElapsedTimef() - timeStarted) - animationTime);
        }
        return ((ofGetElapsedTimef() - timeStarted) < animationTime);
    };
    
    void animate() {
        // This isn't quite there yet.
        // The animation is need to be called from the outside.
        
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
    T inner;
    T target;
    T previous;
    double animationTime, timeStarted;
};

typedef Animatable<ofPoint> ofPointF;
typedef Animatable<ofColor> ofColorF;

}

#endif
