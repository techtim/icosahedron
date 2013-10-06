//
//  Scene.h
//  Icosahedron
//
//  Created by Tim Tavli on 05.10.13.
//
//

#ifndef __Icosahedron__Scene__
#define __Icosahedron__Scene__

#include "testApp.h"

class Scene {
    float  lowFol, midFol, hiFol;
    int lowTrig, midTrig, hiTrig;
    vector<ofColor> baseColors;
    
public:
    Scene();
    virtual void draw();
    void updateParams(vector<ofColor> variableColors) {
        baseColors = variableColors;
    };
//    void updateAudio(float _lowFol, int _lowTrig, float _midFol, int _midTrig, float _hiFol, int _hiTrig):
//        lowFol(_lowFol), lowTrig(_lowTrig), midFol(_midFol), midTrig(_midTrig), hiFol(_hiFol), hiTrig(_hiTrig) {}
    
};

#endif /* defined(__Icosahedron__Scene__) */
