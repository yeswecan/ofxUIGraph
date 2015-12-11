//
// Static UI constraint solver class
//

#ifndef UIConstraintSolver_h
#define UIConstraintSolver_h

#include "UIConstraint.h"

namespace UIGraph {
    class UIConstraintSolver {
    public:
        UIConstraintSolver(){}
        
        // TODO:
        // 1) Make a separate constraint containers for x and for y.
        // 2) Make a function to find and to delete a certain constraint for certain object
        // 3) AddSizeConstraint and AddPositionConstraint should return some kind of reference
        //    you can use to animate or delete them.
        // 4) Use weak_ptr in std::map cause the shape might be deleted at some point.
        //    Make a check.
        
        static void addPositionConstraint(UIShape *obj, float x, float y) {
            positionConstraints[obj] = UIConstraint2D(x, y);
        }
        
        static void addPositionConstraint(UIShape *obj, ofPoint position) {
            positionConstraints[obj] = UIConstraint2D(position.x, position.y);
        }
        
        static void addPositionConstraint(UIShape *obj, UIConstraint2D cc) {
            positionConstraints[obj] = cc;
        }
        
        static void addPositionConstraint(UIShape *obj, UIConstraint X, UIConstraint Y) {
            positionConstraints[obj] = UIConstraint2D(X, Y);
        }
        
        static void addSizeConstraint(UIShape *obj, float x, float y) {
            sizeConstraints[obj] = UIConstraint2D(x, y);
        }
        
        static void addSizeConstraint(UIShape *obj, ofPoint size) {
            positionConstraints[obj] = UIConstraint2D(size.x, size.y);
        }
        
        static void addSizeConstraint(UIShape *obj, UIConstraint2D cc) {
            sizeConstraints[obj] = cc;
        }
        
        static void addSizeConstraint(UIShape *obj, UIConstraint X, UIConstraint Y) {
            sizeConstraints[obj] = UIConstraint2D(X, Y);
        }
        

    protected:
        
        static std::map<UIShape*, UIConstraint2D> positionConstraints, sizeConstraints;
        
        // The following is meant to be called only from canvas class, once in an update maximum
        
        static void solveConstraints() {
            // if it will not compile on windows, replace with smthn like:
            //typedef std::map<std::string, std::map<std::string, std::string>>::iterator it_type;
            for (auto i = positionConstraints.begin(); i != positionConstraints.end(); i++) {
                i->first->position = positionConstraints[i->first].solve();
            }
            for (auto i = sizeConstraints.begin(); i != sizeConstraints.end(); i++) {
                i->first->size = sizeConstraints[i->first].solve();
            }
        }
        

    };
}

#endif /* UIConstraintSolver_h */
