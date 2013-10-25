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
    ofVec3f LoMidHiWaitFrames, pos;

//    icoScene() {;;};
//    ~icoScene() {;;};
    virtual void draw() {;;};
    virtual void update() {;;};
    void updateParams(vector<ofColor> variableColors, ofVec3f _LoMidHiWaitFrames = ofVec3f(60,60,60), ofVec3f _pos = ofVec3f(0,0,0)) {
        for (int i=0; i<variableColors.size(); i++) baseColors.push_back(variableColors[i]);
        LoMidHiWaitFrames = _LoMidHiWaitFrames;
        pos =_pos;
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
     if (midTrig && ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.y) {
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
            ofTranslate(pos.x, pos.y-1.5*fabs(icoUtils::trigFunc(funcCnt,ofGetFrameNum()*.3f*coeff)), pos.y);
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
        is_positive = true;
    };
    void update() {cylMesh1 = ofMesh::cylinder(lowFol, hiFol*2);};
    
    void draw() {
        if (lowTrig && ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.x) {
            lastFrameNum = ofGetFrameNum();
            rotVec = ofVec3f(ofRandom(0, 1), ofRandom(0, 1), 0);
            is_positive = !is_positive;
        }

        if (baseColors.size() >0) ofSetColor(baseColors[0]);
        ofPushMatrix();
            is_positive ?
                ofRotate(60*lowFol, rotVec.x, rotVec.y, rotVec.z):
                ofRotate(-60*lowFol, rotVec.x, rotVec.y, rotVec.z);
            ofTranslate(0, 0, 0);
            cylMesh1.draw();
        
        ofPopMatrix();
    };
    bool  is_positive;
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

        ofTranslate(0, 1.2-hiFol*coeff*7, 0);
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
        boxMesh1 = ofMesh::box(.2, .2, 0.2);
        sphereMesh = ofMesh::sphere(.04);
        sphereMesh2 = ofMesh::sphere(.02);
        sphereMesh3 = ofMesh::sphere(.01);
        
//        spherMesh1 = ofMesh::sphere(.5);
//        baseColors.push_back(ofColor(255,0,0));
//        baseColors.push_back(ofColor(255,255,0));
//        baseColors.push_back(ofColor(0,0, 255));
//        mesh.addVertices(vertices, 8);
//        mesh.addNormals(normals,8);
//        mesh.addIndices(indices, 3*2*6);

        ;
    };
    void update() {
        sphereMesh = ofMesh::sphere(.01+ofMap(lowFol, 0, 1, 0, 0.3));
//        sphereMesh2 = ofMesh::sphere(.02+ofMap(lowFol, 0, 1, 0, 0.2));
//        sphereMesh3 = ofMesh::sphere(.01+ofMap(lowFol, 0, 1, 0, 0.2));
    };

    void draw() {

        if (lowTrig && ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.x) {
            lastFrameNum = ofGetFrameNum();
            vertNums.clear();
            for (int i=0; i < ofMap(coeff, 0, 1, 3, 12); i++)
                vertNums.push_back((int)ofRandom(11));
        }
        
        for (int i=0; i < vertNums.size(); i++) {
            ofPushMatrix();
            ofTranslate(icoMesh.getVertex(vertNums[i]));
//            ofRotate(360*fabs(cos(ofGetFrameNum()*0.02)), 0,1,0);
//            ofTranslate(0, -1.5+1.5*fabs(icoUtils::trigFunc(1, ofGetFrameNum()*0.1*coeff)), 0);
            sphereMesh.draw();

            ofPopMatrix();
        }
            
        
    };
    vector<int> vertNums;
    ofVboMesh boxMesh1, boxMesh2, sphereMesh,sphereMesh2,sphereMesh3;
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
            ofRotate(360*sin(ofGetFrameNum()*0.07*coeff), 0, 1, 0);
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
//                ofSetColor(baseColors[i%baseColors.size()]);
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


class icoScene6 : public icoScene {
public:
    icoScene6() {
        boxMesh1 = ofMesh::box(2, .1, 2);
        lastFrameNum = ofGetFrameNum();
        spherMesh1 = ofMesh::sphere(.1);
        cntr = 4.5;
        //        baseColors.push_back(ofColor(255));
    };
    
    void update() {
        spherMesh1 = ofMesh::sphere(.05+hiFol*0.1);
    };
    void draw() {
        if (lowTrig && ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.x) cntr = 4.5;
            lastFrameNum = ofGetFrameNum();
            cntr-=coeff/4;
            if (cntr < -15) cntr = 5;
            ofPushMatrix();
            ofTranslate(0, cntr/2 , 0);
            boxMesh1.draw();
            ofPopMatrix();
            
//        }
    };
    float cntr;
    ofVboMesh spherMesh1, boxMesh1;
    vector<ofPoint> points;
    
};

class icoScene7 : public icoScene {
public:
    icoScene7() {
//        boxMesh1 = ofMesh::box(.1, 3, 3);
        lastFrameNum = ofGetFrameNum();
        spherMesh1 = ofMesh::sphere(1);
        //        baseColors.push_back(ofColor(255));
        triCntr=0;
    };
    
    void update() {
//        spherMesh1 = ofMesh::sphere(.05+hiFol*0.1);;
//        spherMesh1 = ofMesh::sphere(1);
    };
    void draw() {
        if (ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.x/2) {
            vec = icoMesh.getVertex(Faces[triCntr])+icoMesh.getVertex(Faces[triCntr+1])+icoMesh.getVertex(Faces[triCntr+2]);
            triCntr = triCntr > 60 ? 0 : triCntr+3;
            lastFrameNum = ofGetFrameNum();
        }
        ofPushMatrix();
        ofTranslate(vec);
        //                ofSetColor(baseColors[i%baseColors.size()]);
            spherMesh1.draw();
        ofPopMatrix();

        
    }
    
    int triCntr;
    ofVec3f vec;
    ofVboMesh spherMesh1, boxMesh1;
};

class icoScene8 : public icoScene {
public:
    icoScene8() {
        //        boxMesh1 = ofMesh::box(.1, 3, 3);
        lastFrameNum = ofGetFrameNum();
        spherMesh1 = ofMesh::sphere(0.1*pos.x);
        //        baseColors.push_back(ofColor(255));
        vertCntr=0;
    };
    
    void update() {
        //        spherMesh1 = ofMesh::sphere(.05+hiFol*0.1);;
        spherMesh1 = ofMesh::sphere(hiFol*.5f);
    };
    void draw() {
        if (ofGetFrameNum()-lastFrameNum > LoMidHiWaitFrames.x/2) {
            vec1 = icoMesh.getVertex(Faces[vertCntr]);
            vec2 = icoMesh.getVertex(Faces[vertCntr]+1);
//            icoMesh.getVertex(Faces[vertCntr+1]);
            vertCntr = vertCntr > 60 ? 0 : vertCntr+1;
            lastFrameNum = ofGetFrameNum();
        }
        ofPushMatrix();
        ofTranslate(vec1.interpolate(vec2, .0125*pixCntr++));
        //                ofSetColor(baseColors[i%baseColors.size()]);
        if (pixCntr > 80) pixCntr = 0;
        spherMesh1.draw();
        ofPopMatrix();
    }
    ofVec3f vec1, vec2;
    int vertCntr, pixCntr;
    ofVboMesh spherMesh1, boxMesh1;
};

class icoScene9 : public icoScene {
public:
    icoScene9() {
        //        boxMesh1 = ofMesh::box(.1, 3, 3);
        
        //        baseColors.push_back(ofColor(255));
    };

    void update() {

    };
    void draw() {
        ofPushMatrix();
            ofTranslate(0,0,0);
        ofRotate(360*sin(ofGetFrameNum()*.1*coeff), 0, 1, 0);

        glLineWidth(.3f+lowFol);
        icoMesh.draw();
        ofPopMatrix();
    }
};
//#endif /* defined(__Icosahedron__Scene__) */
