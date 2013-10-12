//
//  kinectScene.h
//  Icosahedron
//
//  Created by Tim Tavli on 10.10.13.
//
//

#ifndef Icosahedron_kinectScene_h
#define Icosahedron_kinectScene_h

class kinectScene {
public:
    float  lowFol, midFol, hiFol;
    int lowTrig, midTrig, hiTrig;
    vector<ofColor> baseColors;
    unsigned int fadingIn, fadingOut;

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
    
    ofVboMesh kinectVerts, spherMesh1, boxMesh1;
    bool isUpdAudio;
    //    ofVboMesh boxMesh1, boxMesh2, boxMesh3, boxMesh4;
    //    ofVboMesh cylMesh1, cylMesh2;
    //    ofVboMesh spherMesh1;
};

class kinectScene1 : public kinectScene {
public:
    kinectScene1() {
        boxMesh1 = ofMesh::box(.1, .1, 0.1);
        spherMesh1 = ofMesh::sphere(.1);
    };
    void update() {;;};
    void draw() {
        ofPushMatrix();
        ofTranslate(1,0,1);
//        ofRotate(360*fabs(cos(ofGetFrameNum()*0.02)), 0,1,0);
        //            ofTranslate(0, -1.5+1.5*fabs(icoUtils::trigFunc(1, ofGetFrameNum()*0.1*coeff)), 0);
        spherMesh1.draw();
        
        ofGetFrameNum();
        
        
        ofPopMatrix();
    };

    
};

#endif
