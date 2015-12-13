#include <stdio.h>
#include "UIObject.h"

//------------------------------- Static vars for finger management

std::map<int, ofPoint>  UIObject::fingerPositions;
std::map<int, bool> UIObject::fingerIsDown;


//------------------------------- Building UI

UIObject* UIObject::addChild(UIObject *object)  {
    ofLog() << "adding " << object->name << "...";
    object->parent = this;
    object->topParent = topParent;
    if (object->zIndex == 0)
        object->zIndex = zIndex++;
    children.push_back(object);
    //ofLog() << "result - " << children.back()->name << "(" << children.back()->zIndex << ")";
    //object->zIndex = 52;
    return children.back();
};


//------------------------------- Navigation and file management

UIObject* UIObject::getChild(string iD) {
    for (auto i: children) {
        if (i->name == iD) return i;
        if (i->getChild(iD) != NULL) return i->getChild(iD);
    }
    return NULL;
}


void UIObject::addXML(ofxXmlSettings *addition, UIObject *targetParent)
{
    for (auto i = 0; i < addition->getNumTags("object"); i++) {
        ofLog() << "object " << addition->getAttribute("object", "name", "<unknown>", i);
        UIObject *addon = targetParent->addChild(new UIObject());
        addon->name = addition->getAttribute("object", "name", "<unknown>", i);
        addon->zIndex = addition->getAttribute("object", "z-index", 0, i);
        if (addition->getAttribute("object", "z-index", 999, i) == 999) {
            if (targetParent != NULL)
                addon->zIndex = targetParent->zIndex;
                };
        string defaultState = addition->getAttribute("object", "default_state", "default", i);
        // tree structure support:
        addition->pushTag("object", i);
        
        if (addition->getNumTags("position") > 0) {
            ofLog() << ".. position found:" << addition->getAttribute("position", "x", 0) << "x" << addition->getAttribute("position", "y", 0);
            addon->position = ofPoint(addition->getAttribute("position", "x", 0), addition->getAttribute("position", "y", 0));
        }
        if (addition->getNumTags("bounds") > 0) {
            addon->position = ofPoint(addition->getAttribute("bounds", "x", 0), addition->getAttribute("bounds", "y", 0));
            addon->size = ofPoint(addition->getAttribute("bounds", "w", 0), addition->getAttribute("bounds", "h", 0));
        }
        if (addition->getNumTags("size") > 0) {
            ofLog() << ".. size found:" << addition->getAttribute("size", "x", 0) << "x" << addition->getAttribute("size", "y", 0);
            addon->size = ofPoint(addition->getAttribute("size", "x", 0), addition->getAttribute("size", "y", 0));
        }
        
        for (auto j = 0; j < addition->getNumTags("state"); j++) {
            ofLog() << ".....state " << addition->getAttribute("state", "name", "", j);
            string stateName = addition->getAttribute("state", "name", "", j);
            addon->data[stateName] = Json::objectValue;
            addition->pushTag("state", j);
            if (addition->getNumTags("position") > 0) {
                addon->data[stateName]["position"] = Json::objectValue;
                addon->data[stateName]["position"]["x"] = addition->getAttribute("position", "x", 0);
                addon->data[stateName]["position"]["y"] = addition->getAttribute("position", "y", 0);
                
                ofLog() << "   ::: position found:" << addon->data[stateName]["position"]["x"].asInt() << "x" << addon->data[stateName]["position"]["y"].asInt();
            }
            if (addition->getNumTags("size") > 0) {
                addon->data[stateName]["size"] = Json::objectValue;
                addon->data[stateName]["size"]["x"] = addition->getAttribute("size", "x", 0);
                addon->data[stateName]["size"]["y"] = addition->getAttribute("size", "y", 0);
                ofLog() << "   ::: size found:" << addon->data[stateName]["size"]["x"].asInt() << "x" << addon->data[stateName]["size"]["y"].asInt();
            }
            addition->popTag();
        }
        
        // test:
        addXML(addition, addon);
        
        addition->popTag();
        
    }
    
}

void UIObject::parseXML(string filename) {
    ofxXmlSettings settings;
    settings.load(filename);
    addXML(&settings, this);
}

void UIObject::parseJSON(string filename) {
    ofxJSONElement file; file.open(filename);
    for (auto i: file.getMemberNames()) { //top-level nodes are always Objects*
        UIObject *addon = new UIObject(ofPoint(0,0), ofPoint(0,0));
        addon->name = i;
        if (file[i].isMember("z-index"))
            addon->zIndex = file[i]["z-index"].asInt();
        if (file[i].isMember("states"))
            addon->data = file[i]["states"];
        if (file[i].isMember("size")) {
            addon->size = ofPoint(
                                  file[i]["size"].isMember("x") ? file[i]["size"]["x"].asInt() : 0,
                                  file[i]["size"].isMember("y") ? file[i]["size"]["y"].asInt() : 0,
                                  file[i]["size"].isMember("z") ? file[i]["size"]["z"].asInt() : 0
                                  );
        }
        children.push_back(addon);
        if (file[i].isMember("default_state")) {
            //                addon->gotoState(file[i]["default_state"].asString(), 0);
        }
    }
}



// ---------------------------  App lifecycle

void UIObject::updateCycle(bool updateAnimator) {
    for (int i = 0; i < children.size(); i++) {
//    for (auto &i: children) {
        children[i]->transformedPosition = transformedPosition + children[i]->position + innerTransform;
        //i->touchPoint = touchPoint;
        children[i]->touchingNow = touchingNow;
        children[i]->updateCycle(false);
        
        // TODO1: animate all animatables here
        children[i]->position.animate();
        if (children[i]->useFbo) {
            if ((children[i]->myFbo.getWidth() != children[i]->size.x) ||
                (children[i]->myFbo.getHeight() != children[i]->size.y)) {
                children[i]->setFboSize(ofPoint(ofGetWidth(), ofGetHeight()));
            }
        }

//    }
    }
    update(this);
    updateWithoutContext();
    executeCallbacks(UIObject::UPDATE, this);
}

void UIObject::drawCycle() {
    if (!visible) return;
    draw(this);
    drawWithoutContext();
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

//--------------------------- Utils

bool UIObject::clippingTest(UIObject *i) {
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
