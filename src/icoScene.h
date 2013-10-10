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

//    icoScene() {;;};
//    ~icoScene() {;;};
    virtual void draw() {;;};
    void updateParams(vector<ofColor> variableColors) {
        baseColors = variableColors;
    };
    void updateAudio(float _lowFol, int _lowTrig, float _midFol, int _midTrig, float _hiFol, int _hiTrig){
        isUpdAudio = true;
        lowFol=_lowFol, lowTrig=_lowTrig, midFol=_midFol, midTrig=_midTrig, hiFol=_hiFol, hiTrig=_hiTrig;
        isUpdAudio = false;
    };
    void updateKinect(ofVboMesh _kinectVerts){
        kinectVerts = _kinectVerts;
    };

    ofVboMesh kinectVerts;
    bool isUpdAudio;
//    ofVboMesh boxMesh1, boxMesh2, boxMesh3, boxMesh4;
//    ofVboMesh cylMesh1, cylMesh2;
//    ofVboMesh spherMesh1;
};


class icoScene0 : public icoScene {
public:
    icoScene0() {
        boxMesh1 = ofMesh::box(1.5, 1.5, 0.2);
        boxMesh2 = ofMesh::box(.2, 1.5, 1.5);
        cylMesh1 = ofMesh::cylinder(.4, 2);
    };
    ~icoScene0() {;;};
    void draw() {
//        if (baseColors.size() >0) ofSetColor(baseColors[0]);
        ofPushMatrix();
        
        ofTranslate(0.5*cos(ofGetFrameNum()*.05f), 0.f, 0.5*sin(ofGetFrameNum()*.05f));
        cylMesh1.draw();

        ofPopMatrix();
    };
    
    ofVboMesh boxMesh1, boxMesh2, cylMesh1;
    vector<ofColor> baseColors;
};

// ------
class icoScene1 : public icoScene {
public:
    icoScene1() {
        boxMesh1 = ofMesh::box(1.5, 1.5, 0.2);
        boxMesh2 = ofMesh::box(.2, 1.5, 1.5);
        cylMesh1 = ofMesh::cylinder(1, 2);
    };
    ~icoScene1() {;;};
    void draw() {
        if (baseColors.size() >0) ofSetColor(baseColors[0]);
        ofPushMatrix();

            ofTranslate(fabs(tan(ofGetFrameNum()*0.02)), -.7, 0);
            cylMesh1.draw();
        
        ofPopMatrix();
    };
    
    ofVboMesh boxMesh1, boxMesh2, cylMesh1;
    vector<ofColor> baseColors;
};

// ------
class icoScene2 : public icoScene {
    
public:
    icoScene2() {
        boxMesh1 = ofMesh::box(1.5, 1.5, 0.2);
        boxMesh2 = ofMesh::box(.2, 1.5, 1.5);
    };
    ~icoScene2() {;;};
    void draw() {
        ofPushMatrix();

        ofTranslate(0, -.7+fabs(cos(ofGetFrameNum()*0.05)), 0);
        ofGetFrameNum();
        boxMesh1.box(1.5, 1.5, 0.2);
        
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
    };
    ~icoScene3() {;;};
    void draw() {
        ofPushMatrix();
            ofTranslate(1,0,1);
//            ofRotate(360*fabs(cos(ofGetFrameNum()*0.02)));
            ofTranslate(0, -1.5+1.5*fabs(cos(ofGetFrameNum()*0.05)), 0);
            spherMesh1.draw();
            
            ofGetFrameNum();

            
        ofPopMatrix();
    };
    ofVboMesh boxMesh1, boxMesh2, spherMesh1;
};

// --------
class icoScene4 : public icoScene {
public:
    icoScene4() {
        boxMesh1 = ofMesh::box(.2, 3, 3);
        spherMesh1 = ofMesh::sphere(.2);
        
    };
    ~icoScene4() {;;};
    void draw() {
        ofPushMatrix();
            boxMesh1.getCentroid().perpendicular(<#const ofVec3f &vec#>) , boxMesh1.getCentroid().y, boxMesh1.getCentroid().z)
            ofRotate(360*sin(ofGetFrameNum()*0.05), boxMesh1.getCentroid().x, boxMesh1.getCentroid().y, boxMesh1.getCentroid().z);
            ofTranslate(1.9*ofVec3f(sin(ofGetFrameNum()*0.1), 0, 0));
            boxMesh1.draw();
        ofPopMatrix();
    }
    ofVboMesh boxMesh1, boxMesh2, spherMesh1;
    
};


class icoScene5 : public icoScene {
public:
    icoScene5() {
        points.assign(10,ofPoint());
        rnd();
        lastFrameNum = ofGetFrameNum();
        spherMesh1 = ofMesh::sphere(.1);
    };
    
    ~icoScene5() {;;};
    void draw() {
        if (!isUpdAudio && lowTrig && ofGetFrameNum()-lastFrameNum > 100) {
            
            rnd();
        }
        
        ofSetColor(0, 200, 230);
        for (int i=0; i<points.size(); i++) {
            ofPushMatrix();
                ofTranslate(points[i]);
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
    int lastFrameNum;
    
};


//#endif /* defined(__Icosahedron__Scene__) */
