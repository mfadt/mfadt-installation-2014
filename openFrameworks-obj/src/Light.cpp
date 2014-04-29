//
//  Light.cpp
//  openFrameworks
//
//  Created by Owen Herterich on 4/27/14.
//
//

#include "Light.h"

Light::Light() {
    radius = 300.0;
    
    light.setDiffuseColor( color );
    light.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
}

void Light::enable(){
    pos.x = ofMap(tiltLR, -180, 180, -400, 400);
    pos.y = ofMap(tiltFB, -90, 90, -400, 400);
    
    light.setDiffuseColor( color );
    light.setPosition( pos );
    light.enable();
}