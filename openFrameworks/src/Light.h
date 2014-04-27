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
        Light( string _userId, int r, int g, int b );
        void update( float tiltLR, float tiltFB );
        void draw();
    
        string userId;
        ofLight light;
        ofVec3f pos;
        ofColor c;
        float radius;
    
    
};
