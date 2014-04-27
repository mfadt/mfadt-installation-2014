#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"

#include "Light.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
	ofxLibwebsockets::Client client;
    
	// websocket methods
	void onConnect( ofxLibwebsockets::Event& args );
	void onOpen( ofxLibwebsockets::Event& args );
	void onClose( ofxLibwebsockets::Event& args );
	void onIdle( ofxLibwebsockets::Event& args );
	void onMessage( ofxLibwebsockets::Event& args );
	void onBroadcast( ofxLibwebsockets::Event& args );
	
	float radius, angle;
    
    float amp;
    
    float tiltLR, tiltFB, dir;
    
    // 3d
    ofEasyCam cam;    
    ofIcoSpherePrimitive sphere;

    //Users
    void newUser( string user, int r, int g, int b );
    vector<Light> clients;
    
};
