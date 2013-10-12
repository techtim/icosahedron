//
//  Particle.cpp
//  Icosahedron
//
//  Created by Tim Tavli on 10.10.13.
//
//

#include "Particle.h"

//コンストラクタ(初期化)
Particle::Particle(){
	setInitialCondition(ofVec3f(0), ofVec3f(0));
	damping = 0.01f;
}

//力(加速度)をリセット
void Particle::resetForce(){
    frc.set(0,0, 0);
}

//力を加える
void Particle::addForce(ofVec3f force){
    frc.x += force.x;
    frc.y += force.y;
    frc.y += force.z;
}

//抵抗力の計算
void Particle::addDampingForce(){
    frc.x = frc.x - vel.x * damping;
    frc.y = frc.y - vel.y * damping;
    frc.z = frc.z - vel.z * damping;
}

//初期状態を設定
void Particle::setInitialCondition(ofVec3f _pos, ofVec3f _vec){
    pos.set(_pos);
	vel.set(_vec);
}

//更新
void Particle::update(){
	vel = vel + frc;
	pos = pos + vel;
}

//描画
void Particle::draw(){
    ofCircle(pos.x, pos.y, 3);
}

ofVec3f Particle::get(){
    return pos;
}
