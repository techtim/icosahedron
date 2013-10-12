//
//  Scene.h
//  Icosahedron
//
//  Created by Tim Tavli on 05.10.13.
//
//

//#ifndef __Icosahedron__Scene__
//#define __Icosahedron__Scene__
#pragma once

#include "testApp.h"

class icoScene {
public:
    float  lowFol, midFol, hiFol;
    int lowTrig, midTrig, hiTrig;
    vector<ofColor> baseColors;
    unsigned int fadingIn, fadingOut;
    ofVec3f LoMidHiWaitFrames;

//    icoScene() {;;};
//    ~icoScene() {;;};
    virtual void draw() {;;};
    virtual void update() {;;};
    void updateParams(vector<ofColor> variableColors, ofVec3f _LoMidHiWaitFrames = ofVec3f(60,60,60)) {
        for (int i=0; i<variableColors.size(); i++) baseColors.push_back(variableColors[i]);
        LoMidHiWaitFrames = _LoMidHiWaitFrames;
    };
    void updateAudio(float _lowFol, int _lowTrig, float _midFol, int _midTrig, float _hiFol, int _hiTrig){
        isUpdAudio = true;
        lowFol=_lowFol, lowTrig=_lowTrig, midFol=_midFol, midTrig=_midTrig, hiFol=_hiFol, hiTrig=_hiTrig;
        isUpdAudio = false;
        update();
    };
    void updateKinect(ofVboMesh _kinectVerts){
        kinectVerts = _kinectVerts;
    };
    
    void setIcoMesh(ofVboMesh _icoMesh) { icoMesh = _icoMesh; };
    void updateCoeff(float _coeff) { coeff = _coeff; };
    
    ofVboMesh kinectVerts, icoMesh;
    bool isUpdAudio;
    float coeff;
    int lastFrameNum, state;
//    ofVboMesh boxMesh1, boxMesh2, boxMesh3, boxMesh4;
//    ofVboMesh cylMesh1, cylMesh2;
//    ofVboMesh spherMesh1;
};


class icoScene0 : public icoScene {
public:
    icoScene0() {
        boxMesh1 = ofMesh::box(1.5, 1.5, 0.1);
        boxMesh2 = ofMesh::box(1.5, .1, 1.5);
        cylMesh1 = ofMesh::cylinder(.1, 2);
        funcCnt = 0;
        lastFrameNum=ofGetFrameNum();
    };
    void update() {
        cylMesh1 = ofMesh::cylinder(.1 +.3*lowFol, 2);
        ;
    };

    void draw() {
//        if (baseColors.size() > 0) ofDrawBitmapString(ofToString(baseColors[0]), 200, 500);
        if (baseColors.size() > 0) ofSetColor(baseColors[0]);
     if (hiTrig && ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.z) {
         funcCnt++;
         funcCnt = funcCnt > 4 ? 0 : funcCnt+1;
         lastFrameNum=ofGetFrameNum();
     }
        ofPushMatrix();
//        ofRotate(midFol*45, 0.5*sin(ofGetFrameNum()*.05f), 0, 0.5*cos(ofGetFrameNum()*.05f));
        ofTranslate(0.8*cos(ofGetFrameNum()*.5f*coeff), 0.f, 0.8*sin(ofGetFrameNum()*.5f*coeff));
        
        cylMesh1.draw();

        ofPopMatrix();
        
        ofPushMatrix();
            ofTranslate(0, 1.2-1.5*fabs(icoUtils::trigFunc(funcCnt,ofGetFrameNum()*.3f*coeff)), 0);
            boxMesh2.draw();
        ofPopMatrix();
    };
    int funcCnt;
    ofVboMesh boxMesh1, boxMesh2, cylMesh1;
    vector<ofColor> baseColors;
};

// ------
class icoScene1 : public icoScene {
public:
    icoScene1() {
        boxMesh1 = ofMesh::box(1.5, 1.5, 0.1);
        boxMesh2 = ofMesh::box(.1, 1.5, 1.5);
        cylMesh1 = ofMesh::cylinder(0.2, 2);\
//        baseColors.push_back(ofColor(255));
        
    };
    void update() {cylMesh1 = ofMesh::cylinder(lowFol, hiFol*2);};
    
    void draw() {
        if (lowTrig && ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.x) {
            lastFrameNum = ofGetFrameNum();
            rotVec = ofVec3f(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1));
        }

        if (baseColors.size() >0) ofSetColor(baseColors[0]);
        ofPushMatrix();
            ofRotate(30*lowFol, rotVec.x, rotVec.y, rotVec.z);
            ofTranslate(0, 0, 0);
            cylMesh1.draw();
        
        ofPopMatrix();
    };
    
    ofVec3f rotVec;
    ofVboMesh boxMesh1, boxMesh2, cylMesh1;
    vector<ofColor> baseColors;
};

// ------
class icoScene2 : public icoScene {
    
public:
    icoScene2() {
        boxMesh1 = ofMesh::box(1.5, .2, 1.5);
        boxMesh2 = ofMesh::box(.2, 1.5, 1.5);
    };
    void update() {;;};
    void draw() {
        ofPushMatrix();

        ofTranslate(0, .8-hiFol*coeff, 0);
        ofGetFrameNum();
        boxMesh1.draw();
        
        ofPopMatrix();
    };
    ofVboMesh boxMesh1, boxMesh2, cylMesh1;
};
//-------

class icoScene3 : public icoScene {
public:
    icoScene3() {
        boxMesh1 = ofMesh::box(1.5, 1.5, 0.2);
        spherMesh1 = ofMesh::sphere(.5);
//        baseColors.push_back(ofColor(255,0,0));
//        baseColors.push_back(ofColor(255,255,0));
//        baseColors.push_back(ofColor(0,0, 255));
//        mesh.addVertices(vertices, 8);
//        mesh.addNormals(normals,8);
//        mesh.addIndices(indices, 3*2*6);

        ;
    };
    void update() {;;};
    void draw() {
        ofPushMatrix();
            ofTranslate(1,0,1);
            ofRotate(360*fabs(cos(ofGetFrameNum()*0.02)), 0,1,0);
//            ofTranslate(0, -1.5+1.5*fabs(icoUtils::trigFunc(1, ofGetFrameNum()*0.1*coeff)), 0);
            spherMesh1.draw();
            
            ofGetFrameNum();
        
            
        ofPopMatrix();
    };
    ofVboMesh boxMesh1, boxMesh2, spherMesh1;
    ofMesh mesh;
};

// --------
class icoScene4 : public icoScene {
public:
    icoScene4() {
        boxMesh1 = ofMesh::box(.2, 3, 3);
        spherMesh1 = ofMesh::sphere(.2);
        
    };
    void update() {;;};
    void draw() {
        ofPushMatrix();
//            boxMesh1.getCentroid().perpendicular(<#const ofVec3f &vec#>) , boxMesh1.getCentroid().y, boxMesh1.getCentroid().z)
            ofRotate(360*sin(ofGetFrameNum()*0.03), 0, 1, 0);
            ofTranslate(1.9*ofVec3f(sin(ofGetFrameNum()*0.1), 0, 0));
            boxMesh1.draw();
        ofPopMatrix();
    }
    float speedCoef;
    ofVboMesh boxMesh1, boxMesh2, spherMesh1;
    
};


class icoScene5 : public icoScene {
public:
    icoScene5() {
        points.assign(20,ofPoint());
        rnd();
        lastFrameNum = ofGetFrameNum();
        spherMesh1 = ofMesh::sphere(.1);
//        baseColors.push_back(ofColor(255));
    };
    
    void update() {
        spherMesh1 = ofMesh::sphere(.05+hiFol*0.1);
    };
    void draw() {
        if (lowTrig && ofGetFrameNum()-lastFrameNum > 30) {
            lastFrameNum = ofGetFrameNum();
            rnd();
        }
        
//        ofSetColor(0, 200, 230);
        for (int i=0; i<points.size(); i++) {
            ofPushMatrix();
                ofTranslate(points[i]);
                ofSetColor(baseColors[i%baseColors.size()]);
                spherMesh1.draw();
            ofPopMatrix();

        }
    };
    
    void rnd() {
        for (int i=0; i<points.size(); i++) {
            ofPoint &p = points[i];
            p.x = ofRandomf();         //trekt een getal tussen -1 en 1
            p.y = ofRandomf();
            p.z = ofRandomf();
            p /= sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
        }
        
    };

    ofVboMesh spherMesh1;
    vector<ofPoint> points;
    
};


//#endif /* defined(__Icosahedron__Scene__) */
