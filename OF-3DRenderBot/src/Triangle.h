//
//  Triangle.h
//  OF-3DRenderBot
//
//  Created by Patricio Gonzalez Vivo on 5/2/14.
//
//

#pragma once

#include "SuperParticle.h"

struct Triangle : public SuperParticle {
    aPoint a,b,c;
    ofPoint tA,tB,tC;
    aPoint normal,original_normal;
    ofQuaternion rot;
    
    void rotateTo(const Triangle &_other,float _speed);
    void draw();
};