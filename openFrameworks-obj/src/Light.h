//
//  Light.h
//  openFrameworks
//
//  Created by Owen Herterich on 4/27/14.
//
//

#pragma once

#include "ofMain.h"

class Light {
public:
    
    Light();
    void update( float tiltLR, float tiltFB );
    void enable();
    void disable();
    
    ofColor color;
    
    float tiltLR;
    float tiltFB;
    float radius;
    
private:
    ofLight light;
    ofVec3f pos;
};
