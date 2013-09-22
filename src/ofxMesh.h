//
//  ofxMesh.h
//  icoExplore
//
//  Created by Tim Tavli on 20.09.13.
//
//

#ifndef __icoExplore__ofxMesh__
#define __icoExplore__ofxMesh__

#include <iostream>
#include "ofMain.h"

class ofxMesh: public ofMesh {
public:
	
	ofxMesh();
    ofxMesh(ofPrimitiveMode mode, const vector<ofVec3f>& verts);
    ~ofxMesh();
    
    ofxMesh xbox( float width, float height, float depth, int resX=2, int resY=2, int resZ=2 );
    void drawInMesh(ofMesh mesh);
};

#endif /* defined(__icoExplore__ofxMesh__) */
