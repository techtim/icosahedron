//
//  Particle.h
//  Icosahedron
//
//  Created by Tim Tavli on 10.10.13.
//
//

#pragma once

#include "ofMain.h"

class Particle {
public:
	
    ofVec3f pos; //位置
    ofVec3f vel; //速度
    ofVec3f frc; //力(加速度)
    float damping; //摩擦力
    
    Particle();
    virtual ~Particle(){};
    void resetForce();
    void addForce(ofVec3f force);
    void addDampingForce();
    void setInitialCondition(ofVec3f _pos, ofVec3f _vec);
    void update();
    void draw();
    ofVec3f get();
    
protected:
private:
};

