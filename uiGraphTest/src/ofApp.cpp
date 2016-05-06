#include "ofApp.h"

using namespace UIGraph;

//--------------------------------------------------------------
void ofApp::setup(){
    
    DrawArgs dArgs;
    dArgs = {new UIObject(), ofPoint(0), ofPoint(300)};
    
    ofParameterGroup g;
    g.
    
    font.loadFont("3_font.ttf", 15);
    
    o1 = ui.addChild(new UIObject("o1"));
    o1->draw = [&](UIObject *o) {
        ofSetColor(255, 0, 0);
        ofRect(0, 0, o1->size.x, o1->size.y);
    };
    o1->setSize(150, 75);
    o1->setPosition(o1->getConstraintInsideRight(ui, 50),
                    o1->getConstraintInsideBottom(ui, 50));
    
    
    o2 = ui.addChild(new UIObject("o2"));
    o2->draw = [&](UIObject *o) {
        ofSetColor(0, 255, 0);
        ofRect(0, 0, o->size.x, o->size.y);
        ofSetColor(255);
        ofDrawBitmapString("o2", 10, 10);
    };
    o2->setSize(100, 75);
    o2->setPosition(o2->getConstraintLeft(o1, 50),
                    o2->getConstraintParallelToTop(o1, 0));
    
    
    
    panel = ui.addChild(new UIObject("panel"));
    panel->draw = [&](UIObject* o) {
        ofSetColor(150, 75, 0, 200);
        ofRect(0,0,o->size.x,o->size.y);
    };
    panel->setSize(panel->getConstraintSizeXMinusArgument(ui, 450),
                   panel->getConstraintSizeYMinusArgument(ui, 100));
    panel->setPosition(UIConstraint(25),UIConstraint(ofRandom(25)));
    panel->useFbo = true;
    panel->setFboSize(ofPoint(ofGetWidth(), ofGetHeight()));
    
    o4 = panel->addChild(new UIObject("o4", ofPoint(75, 75), ofPoint(100, 100)));
    o4->draw = [&](UIObject *o) {
        ofSetColor(100);
        ofRect(0,0,o->size.x,o->size.y);
        ofSetColor(0);
        ofRect(10,10,o->size.x - 20,o->size.y - 20);
        
        ofSetColor(255);
        ofDrawBitmapString("draggable", 11, 20);
        ofDrawBitmapString(o->getTopParent()->name, 11, 30);
    };
    
    //// Gesture stuff for o4
    
    UIDragGestureRecognizer *dgr = new UIDragGestureRecognizer();
    dgr->gestureStarted = [&](UIGestureRecognizer *d) {
        UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
    };
    dgr->gestureUpdated = [&](UIGestureRecognizer *d) {
        UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
        o4->position = UIObject::fingerPositions[dgr->dragFinger] - panel->position - dgr->dragOffset;
        
        if (o4->position.x < 0) o4->position.x = 0;
        if (o4->position.y < 0) o4->position.y = 0;
        
    };
    dgr->gestureEnded = [&](UIGestureRecognizer *d) {
    };
    
    o4->gestureRecognizer = dgr;
    
    // TODO: zIndex manupulations lead to partially inverted drawing coords
//    o4->getToTheFront();
//    o4->zIndex = 81;
    
    //// Editables
    
    e = (EditableField*)panel->addChild(new EditableField("editable", ofPoint(300, 100), ofPoint(130, 30)));
    e->innards = "125";
    e->numbersOnly = true;
    e->setupForDragging(15, 1000);
    e->onNumberChange = [&]() {
        
    };

    
    e2 = (EditableField*)panel->addChild(new EditableField("editable 2", ofPoint(300, 150), ofPoint(130, 30)));
    e2->innards = "string";


    //// Dropdown
    ddl = (UIDropDownList*)panel->addChild(new UIDropDownList("dropdown", &dropdownOptions));
    ddl->font = &font;
    UIConstraintSolver::addPositionConstraint(ddl, 260, 260);
    UIConstraintSolver::addSizeConstraint(ddl, 150, 30);
    for (int i = 0; i < 80; i++) {
        dropdownOptions.push_back(ofToString(ofRandom(50000)));
    }
    ddl->rowHeight = 30;
    
    ///////// Setup

    
    ofSetEscapeQuitsApp(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    ui.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    ui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    UIKeyboardEventReciever::broadcastKeyPressed(key);
    
    if (key == 27) {
        UIKeyboardEventReciever::unfocusKeyboardRecievers();
    }
    
    ofLog() << key;
}


