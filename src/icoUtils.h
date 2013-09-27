#pragma once

#include "ofMain.h"

const ofIndexType Faces[] = {
    2, 1, 0,
    3, 2, 0,
    4, 3, 0,
    5, 4, 0,
    1, 5, 0,
    11, 6,  7,
    11, 7,  8,
    11, 8,  9,
    11, 9,  10,
    11, 10, 6,
    1, 2, 6,
    2, 3, 7,
    3, 4, 8,
    4, 5, 9,
    5, 1, 10,
    2,  7, 6,
    3,  8, 7,
    4,  9, 8,
    5, 10, 9,
    1, 6, 10 };
const float Verts[] = {
    0.000f,  0.000f,  1.000f,
    0.894f,  0.000f,  0.447f,
    0.276f,  0.851f,  0.447f,
    -0.724f,  0.526f,  0.447f,
    -0.724f, -0.526f,  0.447f,
    0.276f, -0.851f,  0.447f,
    0.724f,  0.526f, -0.447f,
    -0.276f,  0.851f, -0.447f,
    -0.894f,  0.000f, -0.447f,
    -0.276f, -0.851f, -0.447f,
    0.724f, -0.526f, -0.447f,
    0.000f,  0.000f, -1.000f };

const ofVec3f icoVerts[] = {
    ofVec3f(0.000f,  0.000f,  1.000f),
    ofVec3f(0.894f,  0.000f,  0.447f),
    ofVec3f(0.276f,  0.851f,  0.447f),
    ofVec3f(-0.724f,  0.526f,  0.447f),
    ofVec3f(-0.724f, -0.526f,  0.447f),
    ofVec3f(0.276f, -0.851f,  0.447f),
    ofVec3f(0.724f,  0.526f, -0.447f),
    ofVec3f(-0.276f,  0.851f, -0.447f),
    ofVec3f(-0.894f,  0.000f, -0.447f),
    ofVec3f(-0.276f, -0.851f, -0.447f),
    ofVec3f(0.724f, -0.526f, -0.447f),
    ofVec3f(0.000f,  0.000f, -1.000f)
};

const ofVec3f icoIndices[] = {
    ofVec3f(2, 1, 0),
    ofVec3f(3, 2, 0),
    ofVec3f(4, 3, 0),
    ofVec3f(5, 4, 0),
    ofVec3f(1, 5, 0),
    ofVec3f(11, 6,  7),
    ofVec3f(11, 7,  8),
    ofVec3f(11, 8,  9),
    ofVec3f(11, 9,  10),
    ofVec3f(11, 10, 6),
    ofVec3f(1, 2, 6),
    ofVec3f(2, 3, 7),
    ofVec3f(3, 4, 8),
    ofVec3f(4, 5, 9),
    ofVec3f(5, 1, 10),
    ofVec3f(2,  7, 6),
    ofVec3f(3,  8, 7),
    ofVec3f(4,  9, 8),
    ofVec3f(5, 10, 9),
    ofVec3f(1, 6, 10)
};

class icoUtils {
public:

    static ofMesh get_vertices() {
        ofMesh ico;
        ico.addVertices(&icoVerts[0], 12);
        ico.addIndices(&Faces[0], 60);
        return ico;
//        ofxMeshUtils::calcNormals(ico);
//        vector<ofVec3f> vertices = ico.getVertices();
        ofMatrix4x4 ROTX = ofMatrix4x4::newRotationMatrix(10.9f,1,0,0);
        ofMatrix4x4 R = ofMatrix4x4::newRotationMatrix(18,0,0,1);
        ofMatrix4x4 T = ofMatrix4x4::newTranslationMatrix(ofVec3f(0,0,0));
        
        for (int i=0; i<12; i++) {
            //        vertices[i] =
            
//            ofVec4f myVector = ofVec4f(vertices[i][0], vertices[i][1], vertices[i][2], 1.0f);
            
            // fill myMatrix and myVector somehow
            //        myVector.
            //        vertices[i].r
            ofVec3f transformedVector = icoVerts[i]*R*ROTX*T;

            ico.addVertex(transformedVector);
            //        ico.setVertex(i, ofVec3f(transformedVector.x, transformedVector.y,transformedVector.z));
        }
        ico.addIndices(&Faces[0], 60);
        return ico;
    }

};