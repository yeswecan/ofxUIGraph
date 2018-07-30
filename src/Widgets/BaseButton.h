//
//  BaseButton.h
//  CordsDemo
//
//  Created by zebra on 15/06/2018.
//

#ifndef BaseButton_h
#define BaseButton_h

#include "ofxUIGraph.h"
#include "ofPoint.h"

class BaseButton: public UIObject {
public:
    BaseButton() {
        
    }
    
    void initMesh(int w, int h) {
        screenMesh.addVertex(ofPoint(0, 0));
        screenMesh.addTexCoord(glm::vec2(0, 0));
        screenMesh.addVertex(ofPoint(w, 0));
        screenMesh.addTexCoord(glm::vec2(1, 0));
        screenMesh.addVertex(ofPoint(w, h));
        screenMesh.addTexCoord(glm::vec2(1, 1));
        
        
        screenMesh.addVertex(ofPoint(0, 0));
        screenMesh.addTexCoord(glm::vec2(0, 0));
        screenMesh.addVertex(ofPoint(w, h));
        screenMesh.addTexCoord(glm::vec2(1, 1));
        screenMesh.addVertex(ofPoint(0, h));
        screenMesh.addTexCoord(glm::vec2(0, 1));
        
        screenMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    }
    
    BaseButton(ofPoint a, ofPoint b, bool fbo = false) : UIObject(a, b) {
        if (fbo) {
            allocateInnerFbo(b.x, b.y);
            
            circleShader.load("circle");
            
            initMesh(b.x, b.y);
    
            useFbo = false;
        }
        
//        update = [&](UIObject *o) {
//            auto a = ofRectangle(o->position.x, o->position.y, o->size.x, o->size.y);
//            auto b = ofPoint(ofGetMouseX(), ofGetMouseY());
//            if  (ofRectangle(o->position.x, o->position.y, o->size.x, o->size.y).inside
//
//                 (ofPoint(ofGetMouseX(), ofGetMouseY()))) {
//                fadeUp();
//            } else fadeDown();
//        };
        
    }
    
    void allocateInnerFbo(int w, int h) {
        innerFbo.allocate(w, h);
        
    }
    
    void startInnerFbo() {
        innerFbo.begin();
        ofBackground(0, 0, 0, 0);
    }
    
    void endInnerFbo() {
        innerFbo.end();
    }
    
    void drawInnerFbo(bool circle = false) {
        ofSetColor(255);
        if (!circle) {
            innerFbo.draw(0, 0);
        } else {
            circleShader.begin();
            circleShader.setUniformTexture("textureInput", innerFbo.getTexture(), 0);
            circleShader.setUniform2f("resolution", innerFbo.getWidth(), innerFbo.getHeight());
            screenMesh.draw();
            circleShader.end();
        }
    }
    
    void fadeUp() {
        fade += (1. - fade) / 5.;
    }
    
    void fadeDown() {
        fade -= (fade) / 5.;
    }
    
    ofFbo innerFbo;
    
    float fade = 0.;
    
    ofShader circleShader;
    ofMesh screenMesh;
    

};
#endif /* BaseButton_h */
