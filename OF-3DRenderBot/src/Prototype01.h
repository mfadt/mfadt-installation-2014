//
//  Prototype01.h
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//

#pragma once

#include "UI3DProject.h"
#include "UIShader.h"

#include "ofxFTGL.h"
#include "ofxLibwebsockets.h"
#include "ofxAssimpModelLoader.h"

#include "Triangle.h"

#include "UISEM.h"

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
    void selfDrawOverlay();
    
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
    
    ofTrueTypeFont  font2D;
    string          textName, textProject;
    
    ofImage         logo;
    
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
    
    vector<string>  cmdBuffer;
    
    UISEM           sem;
    
    //  Flocking
    //
    ofPoint globalOffset;
    float   speed;
    float   targetAttraction, targetRadius, targetTraction;
    float   turbulence,neigbordhood,independence,flocking;
    float   rotateSpeed;
};