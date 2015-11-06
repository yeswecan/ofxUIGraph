#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ui = new UIObject("canvas", ofPoint(0,0), ofPoint(ofGetWidth(), ofGetHeight()));
    ui->draw = [&](UIObject *o) {
        ofSetColor(255);
        ofLine(0, 0, o->size.x, o->size.y);
    };
    ofLog() << "ui size = " << ui->size;
    
    o1 = ui->addChild(new UIObject("o1"));
    o1->draw = [&](UIObject *o) {
        ofSetColor(255, 0, 0);
        ofRect(0, 0, o->size.x, o->size.y);
    };
    
    o1->touchDownC = [&](UIObject *o)->bool {
        return true;
    };
    
    s1 = UIConstraint2D(UIConstraint(150),
                        UIConstraint(75));
    p1 = UIConstraint2D(UIConstraint(UIConstraint::INSIDE_RIGHT, ui, o1, 50),
                        UIConstraint(UIConstraint::INSIDE_BOTTOM, ui, o1, 50));
    ConstraintSolver::addSizeConstraint(o1, s1);
    ConstraintSolver::addPositionConstraint(o1, p1);
    
    o2 = ui->addChild(new UIObject("o2"));
    o2->draw = [&](UIObject *o) {
        ofSetColor(0, 255, 0);
        ofRect(0, 0, o->size.x, o->size.y);
    };
    o2->touchDownC = [&](UIObject *o)->bool {
        return true;
    };
    s2 = UIConstraint2D(UIConstraint(100),
                      UIConstraint(75));
    p2 = UIConstraint2D(UIConstraint(UIConstraint::LEFT, o1, o2, 50),
                        UIConstraint(UIConstraint::PARALLEL_TO_TOP, o1, o2, 0)
                        );
    ConstraintSolver::addSizeConstraint(o2, s2);
    ConstraintSolver::addPositionConstraint(o2, p2);
    
    
    o3 = ui->addChild(new UIObject("o3"));
    o3->draw = [&](UIObject* o) {
        ofSetColor(150, 75, 0, 200);
        ofRect(0,0,o->size.x,o->size.y);
    };
    ConstraintSolver::addSizeConstraint(o3,
                        UIConstraint2D(UIConstraint(UIConstraint::SIZE_X_MINUS_ARGUMENT, ui, 450),
                                       UIConstraint(UIConstraint::SIZE_Y_MINUS_ARGUMENT, ui, 100)
                                       ));
    ConstraintSolver::addPositionConstraint(o3, UIConstraint2D(UIConstraint(25),UIConstraint(25)));
    o3->useFbo = true;
    o3->setFboSize(ofPoint(ofGetWidth(), ofGetHeight()));
    
    o4 = o3->addChild(new UIObject("o4", ofPoint(75, 75), ofPoint(100, 100)));
    o4->draw = [&](UIObject *o) {
        ofSetColor(100);
        ofRect(0,0,o->size.x,o->size.y);
        ofSetColor(0);
        ofRect(10,10,o->size.x - 20,o->size.y - 20);
        
        ofSetColor(255);
        ofDrawBitmapString("draggable", 11, 20);
    };
    
    //// Gesture stuff for o4
    
    UIDragGestureRecognizer *dgr = new UIDragGestureRecognizer();
    dgr->gestureStarted = [&](UIGestureRecognizer *d) {
        UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
    };
    dgr->gestureUpdated = [&](UIGestureRecognizer *d) {
        UIDragGestureRecognizer *dgr = (UIDragGestureRecognizer*)d;
        o4->position = UIObject::fingerPositions[dgr->dragFinger] - o3->position - dgr->dragOffset;
        
        if (o4->position.x < 0) o4->position.x = 0;
        if (o4->position.y < 0) o4->position.y = 0;
        
    };
    dgr->gestureEnded = [&](UIGestureRecognizer *d) {
    };
    
    o4->gestureRecognizer = dgr;
    
    //// Editables
    
    e = (EditableField*)o3->addChild(new EditableField("editable", ofPoint(300, 100), ofPoint(130, 30)));
    e->innards = "125";
    e->numbersOnly = true;
    e->setupForDragging(15, 1000);
    e->onNumberChange = [&]() {
        
    };

    
    e2 = (EditableField*)o3->addChild(new EditableField("editable 2", ofPoint(300, 150), ofPoint(130, 30)));
    e2->innards = "string";

    
    //// Dropdown
    ddl = (UIDropDownList*)o3->addChild(new UIDropDownList("dropdown", &dropdownOptions));
    ConstraintSolver::addPositionConstraint(ddl, 260, 260);
    ConstraintSolver::addSizeConstraint(ddl, 150, 30);
    for (int i = 0; i < 70; i++) {
        dropdownOptions.push_back(ofToString(ofRandom(50000)));
    }
    ddl->rowHeight = 30;
    
    ///////// Setup

    UIKeyboardEventReciever::initializeEditableFields();
    ConstraintSolver::solveConstraints();
    
    
    ofSetEscapeQuitsApp(false);
}

//--------------------------------------------------------------
void ofApp::update(){
    ConstraintSolver::solveConstraints();
    ui->updateCycle();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    ui->drawCycle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    UIKeyboardEventReciever::broadcastKeyPressed(key);
    
    if (key == 27) {
        UIKeyboardEventReciever::unfocusKeyboardRecievers();
    }
    
    ofLog() << key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    UIKeyboardEventReciever::broadcastKeyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ui->touchBroadcast(ofPoint(x, y), UIObject::TOUCH_DRAG);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ui->touchBroadcast(ofPoint(x, y), UIObject::TOUCH_DOWN);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ui->touchBroadcast(ofPoint(x, y), UIObject::TOUCH_UP);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ui->size = ofPoint(w, h);
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
