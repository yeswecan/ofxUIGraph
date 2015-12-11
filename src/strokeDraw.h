//
//  strokeDraw.h
//  microFanta
//
//  Created by zebra on 07/04/14.
//
//

#ifndef microFanta_strokeDraw_h
#define microFanta_strokeDraw_h

#include "ofMain.h"

static void drawCornerFrame(ofRectangle bounds, float strokeLength, float strokeWidth) {
    ofFill();
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    ofBeginShape();
    ofVertex(bounds.x, bounds.y);
    ofVertex(bounds.x + strokeLength + strokeWidth, bounds.y);
    ofVertex(bounds.x + strokeLength + strokeWidth, bounds.y + strokeWidth);
    ofVertex(bounds.x + strokeWidth, bounds.y + strokeWidth);
    ofVertex(bounds.x + strokeWidth, bounds.y + strokeLength + strokeWidth);
    ofVertex(bounds.x, bounds.y + strokeLength + strokeWidth);
    ofEndShape();
    
    ofBeginShape();
    ofVertex(bounds.x + bounds.width - strokeLength - strokeWidth, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y + strokeWidth + strokeLength);
    ofVertex(bounds.x + bounds.width - strokeWidth, bounds.y + strokeWidth + strokeLength);
    ofVertex(bounds.x + bounds.width - strokeWidth, bounds.y + strokeWidth);
    ofVertex(bounds.x + bounds.width - strokeWidth - strokeLength, bounds.y + strokeWidth);
    ofEndShape();
    
    ofBeginShape();
    ofVertex(bounds.x, bounds.y + bounds.height - strokeLength - strokeWidth);
    ofVertex(bounds.x + strokeWidth, bounds.y + bounds.height - strokeLength - strokeWidth);
    ofVertex(bounds.x + strokeWidth + strokeLength, bounds.y + bounds.height - strokeWidth);
    ofVertex(bounds.x + strokeWidth + strokeLength, bounds.y + bounds.height);
    ofVertex(bounds.x, bounds.y + bounds.height);
    ofEndShape();
    
    ofBeginShape();
    ofVertex(bounds.x + bounds.width - strokeLength - strokeWidth,
             bounds.y + bounds.height - strokeWidth);
    ofVertex(bounds.x + bounds.width - strokeWidth,
             bounds.y + bounds.height - strokeWidth);
    ofVertex(bounds.x + bounds.width - strokeWidth,
             bounds.y + bounds.height - strokeWidth - strokeLength);
    ofVertex(bounds.x + bounds.width,
             bounds.y + bounds.height - strokeWidth - strokeLength);
    ofVertex(bounds.x + bounds.width,
             bounds.y + bounds.height);
    ofVertex(bounds.x + bounds.width - strokeLength - strokeWidth,
             bounds.y + bounds.height);
    ofEndShape();
    
}

static void drawLineStroke(ofRectangle bounds, float strokeLength, float strokeWidth) {
    
    ofFill();
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    // TOP LEFT CORNER
    ofBeginShape();
    ofVertex(bounds.x, bounds.y);
    ofVertex(bounds.x + strokeLength, bounds.y);
    ofVertex(bounds.x + strokeLength, bounds.y + strokeWidth);
    ofVertex(bounds.x, bounds.y + strokeWidth);
    ofEndShape();
    
    ofBeginShape();
    ofVertex(bounds.x, bounds.y);
    ofVertex(bounds.x + strokeWidth, bounds.y);
    ofVertex(bounds.x + strokeWidth, bounds.y + strokeLength);
    ofVertex(bounds.x, bounds.y + strokeLength);
    ofEndShape();
    
    // BOTTOM LEFT CORNER
    ofBeginShape();
    ofVertex(bounds.x, 				  bounds.y + bounds.height - strokeLength);
    ofVertex(bounds.x + strokeWidth,  bounds.y + bounds.height - strokeLength);
    ofVertex(bounds.x + strokeWidth,  bounds.y + bounds.height);
    ofVertex(bounds.x, 				  bounds.y + bounds.height);
    ofEndShape();
    
    ofBeginShape();
    ofVertex(bounds.x, bounds.y + bounds.height - strokeWidth);
    ofVertex(bounds.x + strokeLength, bounds.y + bounds.height - strokeWidth);
    ofVertex(bounds.x + strokeLength, bounds.y + bounds.height );
    ofVertex(bounds.x, bounds.y + bounds.height);
    ofEndShape();
    
    // TOP RIGHT CORNER
    ofBeginShape();
    ofVertex(bounds.x + bounds.width - strokeLength, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y + strokeWidth);
    ofVertex(bounds.x + bounds.width - strokeLength, bounds.y + strokeWidth);
    ofEndShape();
    
    ofBeginShape();
    ofVertex(bounds.x + bounds.width - strokeWidth, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y + strokeLength);
    ofVertex(bounds.x + bounds.width - strokeWidth, bounds.y + strokeLength);
    ofEndShape();
    
    // BOTTOM RIGHT CORNER
    ofBeginShape();
    ofVertex(bounds.x + bounds.width - strokeLength, bounds.y + bounds.height - strokeWidth);
    ofVertex(bounds.x + bounds.width, bounds.y + bounds.height - strokeWidth);
    ofVertex(bounds.x + bounds.width, bounds.y + bounds.height);
    ofVertex(bounds.x + bounds.width - strokeLength, bounds.y + bounds.height);
    ofEndShape();
    
    ofBeginShape();
    ofVertex(bounds.x + bounds.width - strokeWidth, bounds.y + bounds.height - strokeLength);
    ofVertex(bounds.x + bounds.width, bounds.y + bounds.height - strokeLength);
    ofVertex(bounds.x + bounds.width, bounds.y + bounds.height);
    ofVertex(bounds.x + bounds.width - strokeWidth, bounds.y + bounds.height);
    ofEndShape();
    
    // HORIZONTAL STROKE
    float strokeCount = bounds.width / (strokeLength * 2) - 2;
    for (int i = 0; i < strokeCount; i++) {
        
		//EXPERIMENT: perfomance?
    	if ((bounds.x + strokeLength*2 + i * (strokeLength * 2)) > ofGetWidth() ) { break;}
		//
    	ofBeginShape(); // top
        
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2), bounds.y);
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2) + strokeLength, bounds.y );
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2) + strokeLength, bounds.y + strokeWidth);
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2), bounds.y + strokeWidth );
    	ofEndShape();
        
    	ofBeginShape(); // bottom
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2), bounds.y + bounds.height  - strokeWidth);
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2) + strokeLength, bounds.y  - strokeWidth + bounds.height);
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2) + strokeLength, bounds.y  + bounds.height);
        ofVertex(bounds.x + strokeLength*2 + i * (strokeLength * 2), bounds.y  + bounds.height);
    	ofEndShape();
    }
    
    // VERTICAL STROKE
    strokeCount = bounds.height / (strokeLength * 2) - 2;
    for (int i = 0; i < strokeCount; i++) {
        
		//EXPERIMENT: perfomance?
    	if ((bounds.y + strokeLength*2 + i * (strokeLength * 2)) > ofGetHeight() ) { break;}
		//
        
    	ofBeginShape(); // left
        ofVertex(bounds.x, 				 bounds.y + strokeLength*2 + i * (strokeLength * 2));
        ofVertex(bounds.x + strokeWidth, bounds.y + strokeLength*2 + i * (strokeLength * 2));
        ofVertex(bounds.x + strokeWidth, bounds.y + strokeLength*3 + i * (strokeLength * 2));
        ofVertex(bounds.x, 				 bounds.y + strokeLength*3 + i * (strokeLength * 2));
    	ofEndShape();
        
    	ofBeginShape(); // right
        ofVertex(bounds.x + bounds.width - strokeWidth, bounds.y + strokeLength*2 + i * (strokeLength * 2));
        ofVertex(bounds.x + bounds.width, bounds.y + strokeLength*2 + i * (strokeLength * 2));
        ofVertex(bounds.x + bounds.width, bounds.y + strokeLength*3 + i * (strokeLength * 2));
        ofVertex(bounds.x + bounds.width - strokeWidth, 				 bounds.y + strokeLength*3 + i * (strokeLength * 2));
    	ofEndShape();
        
    }
    
}

static void drawBounds(ofRectangle bounds, int thickness) {
	ofBeginShape();
    ofVertex(bounds.x, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y + thickness);
    ofVertex(bounds.x, bounds.y + thickness);
	ofEndShape();
    
	ofBeginShape();
    ofVertex(bounds.x, bounds.y);
    ofVertex(bounds.x + thickness, bounds.y);
    ofVertex(bounds.x + thickness, bounds.y + bounds.height);
    ofVertex(bounds.x, bounds.y + bounds.height);
	ofEndShape();
    
	ofBeginShape();
    ofVertex(bounds.x + bounds.width - thickness, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y);
    ofVertex(bounds.x + bounds.width, bounds.y + bounds.height);
    ofVertex(bounds.x + bounds.width - thickness, bounds.y + bounds.height);
	ofEndShape();
    
	ofBeginShape();
    ofVertex(bounds.x, bounds.y + bounds.height - thickness);
    ofVertex(bounds.x + bounds.width, bounds.y + bounds.height - thickness);
    ofVertex(bounds.x + bounds.width, bounds.y + bounds.height);
    ofVertex(bounds.x, bounds.y + bounds.height);
	ofEndShape();
}

static void drawBounds(float x, float y, float w, float h, int thickness) {
    drawBounds(ofRectangle(x,y,w,h), thickness);
}

static void drawCells(ofRectangle bounds, int cellSize, float width) {
	float cellCountX = bounds.width / cellSize - 1;
	float cellCountY = bounds.height / cellSize - 1;
	for (int i = 0; i< cellCountX; i++) {
		ofBeginShape();
        ofVertex(bounds.x + (i + 1) * cellSize, bounds.y + 0);
        ofVertex(bounds.x + (i + 1) * cellSize + width, bounds.y + 0);
        ofVertex(bounds.x + (i + 1) * cellSize + width, bounds.y + bounds.height);
        ofVertex(bounds.x + (i + 1) * cellSize, bounds.y + bounds.height);
		ofEndShape();
	}
    
	for (int i = 0; i < cellCountY; i++) {
		ofBeginShape();
        ofVertex(bounds.x, bounds.y + (i + 1) * cellSize);
        ofVertex(bounds.x + bounds.width, bounds.y + (i + 1) * cellSize);
        ofVertex(bounds.x + bounds.width, bounds.y + (i + 1) * cellSize + width);
        ofVertex(bounds.x, bounds.y + (i + 1) * cellSize + width);
		ofEndShape();
	}
}

static void drawStroke(ofRectangle bounds, int width, float cut, float angle) {
    
    ofFill();
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    
    const int wirina = width ;
    const float koli4estvo = bounds.width / width;
    float srez = cut; // from zero to two
    float biasFactor = angle;
    
    float addedStrokes = (float)bounds.height / (biasFactor * wirina);
    
    ofPoint topA, topB, bottomA, bottomB;
    
    for (int i = 0; (float) i < (koli4estvo + addedStrokes); i++) {
        
        topA = ofPoint(bounds.x + (i * 2)     * wirina,  bounds.y);
        topB = ofPoint(bounds.x + (i * 2 + srez) * wirina,  bounds.y);
        bottomA = ofPoint (bounds.x + (i * 2 + srez) * wirina - biasFactor * bounds.height,  bounds.y + bounds.height);
        bottomB = ofPoint (bounds.x + (i * 2)     * wirina - biasFactor * bounds.height,  bounds.y  + bounds.height);
        
        if (bottomA.x < bounds.x) { bottomA.y = bottomA.y - (float)(bounds.x - bottomA.x) / (float)biasFactor;
            bottomA.x = bounds.x;}
        if (bottomB.x < bounds.x) { bottomB.y = bottomB.y - (float)(bounds.x - bottomB.x) / (float)biasFactor;
            bottomB.x = bounds.x; }
        if (topA.x < bounds.x) { topA.x = bounds.x;}
        if (topB.x < bounds.x) { topB.x = bounds.x;}
        
        if (bottomA.x > bounds.x + bounds.width) { //bottomA.y = bottomA.y - (float)(bounds.x + bounds.width - bottomA.x) / (float)biasFactor;
            bottomA.x = bounds.x + bounds.width; }
        if (bottomB.x > bounds.x + bounds.width) { //bottomB.y = bottomB.y - (float)(bounds.x + bounds.width - bottomB.x) / (float)biasFactor;
            bottomB.x = bounds.x + bounds.width; }
        if (topA.x > bounds.x + bounds.width) { topA.y = topA.y - (float)(bounds.x + bounds.width - topA.x) / (float)biasFactor;
            topA.x = bounds.x + bounds.width; }
        if (topB.x > bounds.x + bounds.width) { topB.y = topB.y - (float)(bounds.x + bounds.width - topB.x) / (float)biasFactor;
            topB.x = bounds.x + bounds.width; }
        
        ofBeginShape();
        ofVertex(topA);
        
        if (topB.x >= (bounds.x + bounds.width)) { ofVertex(bounds.x + bounds.width, bounds.y); }
        
        ofVertex(topB);
        
        ofVertex(bottomA);
        
        if ( (bottomA.y >= (bounds.y + bounds.height)) &&  (bottomB.x <= (bounds.x)) ) {
            ofVertex(bounds.x, bounds.y + bounds.height);
        }
        
        ofVertex(bottomB);
        ofEndShape();
    }
    
    
}


#endif
