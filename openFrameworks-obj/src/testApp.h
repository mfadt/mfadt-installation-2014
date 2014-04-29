#pragma once

#include "ofMain.h"

#include "ofxLibwebsockets.h"
#include "ofxAssimpModelLoader.h"

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
    
	// WebSocket
    //
	void onConnect( ofxLibwebsockets::Event& args );
	void onOpen( ofxLibwebsockets::Event& args );
	void onClose( ofxLibwebsockets::Event& args );
	void onIdle( ofxLibwebsockets::Event& args );
	void onMessage( ofxLibwebsockets::Event& args );
	void onBroadcast( ofxLibwebsockets::Event& args );
    
    ofxLibwebsockets::Client client;
	
    //  Clients
    //
    map<string,Light> lights;
    
    // 3D Stuff
    //
    ofEasyCam cam;
    ofMesh  mesh;
    ofTexture texture;

};
