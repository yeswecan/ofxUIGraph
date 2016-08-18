#include <stdio.h>
#include "UIObject.h"

///             Static vars for finger management

std::map<int, ofPoint>  UIObject::fingerPositions;
std::map<int, bool> UIObject::fingerIsDown;


///             Building UI

UIObject* UIObject::addChild(UIObject *object)  {
//    ofLog() << "adding " << object->name << "...";
    object->parent = this;
    if (object->zIndex == 0)
        object->zIndex = zIndex++;
    children.push_back(object);
    //ofLog() << "result - " << children.back()->name << "(" << children.back()->zIndex << ")";
    //object->zIndex = 52;
    return children.back();
};


///             Convenience getter

UIObject* UIObject::getChild(string iD) {
    for (auto i: children) {
        if (i->name == iD) return i;
        if (i->getChild(iD) != NULL) return i->getChild(iD);
    }
    return NULL;
}

///             App lifecycle

void UIObject::updateCycle(bool updateAnimator) {
    for (int i = 0; i < children.size(); i++) {
        children[i]->transformedPosition = transformedPosition + children[i]->position + innerTransform;
        //i->touchPoint = touchPoint;
//        children[i]->touchingNow = touchingNow;
        children[i]->updateCycle(false);
        
        // TODO1: animate all animatables here
        children[i]->position.animate();
        if (children[i]->useFbo) {
            if ((children[i]->myFbo.getWidth() != children[i]->size.x) ||
                (children[i]->myFbo.getHeight() != children[i]->size.y)) {
//                ofLog() << "resizing fbo; current size x = " << children[i]->myFbo.getWidth() << " , required = " << children[i]->size.x;
                children[i]->setFboSize(ofPoint(children[i]->size.x, children[i]->size.y));
            }
        }

//    }
    }
    update(this);
    executeCallbacks(UIObject::UPDATE, this);
}

void UIObject::drawCycle() {
    if (!visible) return;
    draw(this);
    executeCallbacks(UIObject::DRAW, this);
    
    int czIndex = 0;
    int childrenDrawn = 0;
    while (childrenDrawn < children.size()) {
        for (auto &i: children) {
            if (i->zIndex == czIndex) {
                if ((clippingTest(i)) || (bypassClippingTest))
                {
                    if (useViewport) {
                        ofPushView();
                            ofViewport(ofRectangle(i->position.x, i->position.y, i->size.x, i->size.y));
                            ofSetupScreen();
                            i->drawCycle();
                        ofPopView();
                    } else {
                        // Here we transform matrix FROM THE GROUND UP to
                        // support the inner logic of drawing
                        ofPushMatrix();
                        ofTranslate(i->position + innerTransform);
                        if (i->useFbo) {
                            i->myFbo.begin();
                            ofClear(0,0,0,0);
                            i->drawCycle();
                            i->myFbo.end();
                            
                            ofSetColor(255);
                            i->myFbo.getTextureReference().drawSubsection(0, 0, i->size.x, i->size.y, 0, 0, i->size.x, i->size.y);
                        }
                        else {
                            i->drawCycle();
                        }
                        ofPopMatrix();
                    }
                    
                } else {
                }
                childrenDrawn++;
            }
        }
        czIndex++;
    }
}

///             Utils

bool UIObject::clippingTest(UIShape *i) {
    if ((parent!= NULL) && (!parent->clipTestChildren)) return true;
    if (!clipTestMe) return true;
    if (parent == NULL) return true;
    ofRectangle bounds = ofRectangle(- innerTransform.x, - innerTransform.y, size.x, size.y);
    
    if (
        (
         (i->position.x >= bounds.x) ||
         ((i->position.x + i->size.x) >= bounds.x))
        &&
        ((i->position.y >= bounds.y) ||
         ((i->position.y + i->size.y) >= bounds.y))
        && (i->position.x < (bounds.x + bounds.width))
        && (i->position.y < (bounds.y + bounds.height) ))
    {
        return true;
    } else {
        //ofLog() << "object " << i->name << " . bounds:" << bounds;
        return false;
    }
};
