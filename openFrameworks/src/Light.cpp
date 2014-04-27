//
//  Light.cpp
//  openFrameworks
//
//  Created by Owen Herterich on 4/27/14.
//
//

#include "Light.h"

Light::Light( string _userId, int r, int g, int b ) {
    userId = _userId;
    c = ofColor( r, g, b );
    pos.set(0,0, 250);
    radius = 300.0;
    
    light.setDiffuseColor( c );
    light.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
}

void Light::update( float tiltLR, float tiltFB ) {
    //Moving around a sphere (doesn't really work)
//    float theta = -tiltLR;
//    float phi = -tiltFB;
//    
//    pos.z = radius * sin( theta ) * sin( phi );
//    pos.y = radius * cos( theta );
//    pos.x = radius * sin( theta ) * cos( phi );
    
    
    pos.x = ofMap(tiltLR, -180, 180, -400, 400);
    pos.y = ofMap(tiltFB, -90, 90, -400, 400);
}

void Light::draw() {
    light.enable();
    light.setPosition( pos );
}