//
//  Triangle.cpp
//  OF-3DRenderBot
//
//  Created by Patricio Gonzalez Vivo on 5/2/14.
//
//

#include "Triangle.h"

void Triangle::rotateTo(const Triangle &_other,float _speed){
    a.goTo((ofPoint)_other.a,1.0);
    b.goTo((ofPoint)_other.b,1.0);
    c.goTo((ofPoint)_other.c,1.0);
    
    //        if(a != _other.a){
    //            a = _other.a;
    //        } else if(b != _other.b){
    //            b = _other.b;
    //        } else if(c != _other.c){
    //            c = _other.c;
    //        } else
    
    if(tA != _other.tA){
        tA = _other.tA;
    } else if(tB != _other.tB){
        tB = _other.tB;
    } else if(tC != _other.tC){
        tC = _other.tC;
    }
    
    original_normal.goTo((ofPoint)_other.original_normal,0.9);
    rot.slerp(_speed,rot,_other.rot);
}

void Triangle::draw(){
    
    ofQuaternion dir;
    dir.makeRotate(180, acc.getNormalized());
    
    ofPoint A = (ofPoint)*this+dir*rot*a;
    ofPoint B = (ofPoint)*this+dir*rot*b;
    ofPoint C = (ofPoint)*this+dir*rot*c;
    
    normal.set(dir*rot*original_normal);
    normal.normalize();
    
    glNormal3f(normal.x, normal.y, normal.z);
    glTexCoord2f(tA.x, tA.y);
    glVertex3f(A.x, A.y, A.z);
    glNormal3f(normal.x, normal.y, normal.z);
    glTexCoord2f(tB.x, tB.y);
    glVertex3f(B.x, B.y, B.z);
    glNormal3f(normal.x, normal.y, normal.z);
    glTexCoord2f(tC.x, tC.y);
    glVertex3f(C.x, C.y, C.z);
}