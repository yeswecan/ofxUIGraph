#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLog() << plotter.getWindowSize();
    plotter.setWindowSize(2000);
    
    // TODO: easy UIObject constructors with constraints
    // TODO: constraints visualizations for debug
    
    plotterWidget = canvas.addChild(new UIObject("plotter", 0, 0, 320, 240));
    plotterWidget->draw = [&](UIObject *o) {
        plotter.draw(0, 0, o->size.x, o->size.y);
    };
}

//--------------------------------------------------------------
void ofApp::update(){
    plotter["sin1"] << sin(ofGetElapsedTimef() * 1);
    plotter["sin2"] << sin(ofGetElapsedTimef() * 4);
    plotter["sin3"] << sin(ofGetElapsedTimef() * 8);
    
    canvas.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    canvas.draw();
//    plotter.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
