//
//  Prototype01.h
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//

#pragma once

#include "UI3DProject.h"
#include "UI3DGrid.h"
#include "UIShader.h"
#include "SuperParticle.h"

#include "ofxLibwebsockets.h"
#include "ofxAssimpModelLoader.h"

struct Triangle : public SuperParticle {
    aPoint a,b,c;
    aPoint normal;
    ofQuaternion rot;
    
    void rotateTo(const Triangle &_other,float _speed){

        a.goTo((ofPoint)_other.a,9.0);
        b.goTo((ofPoint)_other.b,9.0);
        c.goTo((ofPoint)_other.c,9.0);
        
        normal.goTo((ofPoint)_other.normal,0.5);
        rot.slerp(_speed,rot,_other.rot);
    }
    
    void draw(){
        
        ofPoint A = (ofPoint)*this+rot*a;
        ofPoint B = (ofPoint)*this+rot*b;
        ofPoint C = (ofPoint)*this+rot*c;
        
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(A.x, A.y, A.z);
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(B.x, B.y, B.z);
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(C.x, C.y, C.z);
    }
};

class Prototype01 : public UI3DProject {
public:
    
    string getSystemName(){ return "Prototype01";}
    
    void selfSetupGuis();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
    void selfSetup();
    void selfUpdate();
    
    void selfDraw();
    
	void selfEnd();
    void selfExit();
    
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);
    
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
protected:
  
    void deCompose(ofMesh &_mesh, vector<Triangle>& _triangles);
    
    ofxLibwebsockets::Client client;
  
    UI3DGrid    grid;
    
    vector<string> names;
    int nCounter;
    
    ofMesh  meshTarget;
    
    vector<Triangle> triangles;
    vector<Triangle> trianglesTarget;
    
    ofPoint modelOffset;
    
    ofPoint globalOffset;
    float   speed;
    float   targetAttraction, targetRadius, targetTraction;
    float   turbulence,neigbordhood,independence,flocking;
    float   rotateSpeed;
    
    bool    bNext;
};