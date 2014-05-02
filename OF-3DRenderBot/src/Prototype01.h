//
//  Prototype01.h
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//

#pragma once

#include "UI3DProject.h"
#include "UI3DGrid.h"
#include "SuperParticle.h"
#include "UIShader.h"

#include "ofxFTGL.h"
#include "ofxLibwebsockets.h"
#include "ofxAssimpModelLoader.h"

struct Triangle : public SuperParticle {
    aPoint a,b,c;
    ofPoint tA,tB,tC;
    aPoint normal,original_normal;
    ofQuaternion rot;
    
    void rotateTo(const Triangle &_other,float _speed){
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
    
    void draw(){

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
};

class Prototype01 : public UI3DProject {
public:
    
    string getSystemName(){ return "Prototype01";}
    
    void selfSetupGuis();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
    
    void selfBegin();

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
    ofxLibwebsockets::Client client;
  
    //  3D Elements
    //
    UI3DGrid        grid;
    
    ofxFTGLFont     font;
    float           fontNameSize, fontNameDeep, fontNameAlpha;
    string          text;
    
    //  3D Models
    //
    vector<string>  names;
    int             nCounter;
    bool            bNext;
    
    ofxAssimpModelLoader meshLoader;
    ofMesh              meshTarget;
    vector<Triangle>    triangles;
    vector<Triangle>    trianglesTarget;
    ofTexture           meshTexture;
    ofFbo               meshDimTexture;
    ofPoint             meshOffset;
    float               meshTextureAlpha;
    int                 nFaceCounter;
    int                 nFaceForFrame;
    
    void            terrainMake();
    UIShader        terrainShader;
    ofVboMesh       terrain;
    int             terrainWidth,terrainHeight;
    float           terrainScale, terrainWireframeAlpha;
    
    //  Flocking
    //
    ofPoint globalOffset;
    float   speed;
    float   targetAttraction, targetRadius, targetTraction;
    float   turbulence,neigbordhood,independence,flocking;
    float   rotateSpeed;
};