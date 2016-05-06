#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLog() << plotter.getWindowSize();
    plotter.setWindowSize(2000);
}

//--------------------------------------------------------------
void ofApp::update(){
    plotter["sin1"] << sin(ofGetElapsedTimef() * 1);
    plotter["sin2"] << sin(ofGetElapsedTimef() * 4);
    plotter["sin3"] << sin(ofGetElapsedTimef() * 8);
}

//--------------------------------------------------------------
void ofApp::draw(){
    plotter.draw(0, 0, ofGetWidth(), ofGetHeight());
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
