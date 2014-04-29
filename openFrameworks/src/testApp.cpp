#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetSmoothLighting(true);
    ofEnableDepthTest();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    client.connect("149.31.195.8", 8081);
    ofSetLogLevel(OF_LOG_ERROR);
    
    client.addListener(this);
	
	ofBackground(0);
	ofSetCircleResolution(60);
	ofNoFill();
	
	radius = 300;
	angle = 0;
    amp = 300;
    
    tiltLR = 0;
    tiltFB = 0;
    dir = 0;
    
    //Easy Cam
    cam.setDistance(500);
    
    //Sphere
    sphere.setRadius(150);
    sphere.setPosition(0,0,0);
    ofSetSphereResolution(64);
    
    //149.31.195.8:8080
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofDisableLighting();
    ofSetColor(255);
    ofDrawBitmapString(ofToString(tiltLR) + " | " + ofToString(tiltFB), 20,20);
    
    ofEnableLighting();

	ofPushMatrix();
	{
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        
        cam.begin();
        
        for ( int i = 0; i < clients.size(); i++ ) {
            clients[i].draw();
        }
        
        sphere.draw();
        sphere.drawAxes( 300 );
        
        cam.end();

	}
	ofPopMatrix();
}

void testApp::newUser(string user, int r, int g, int b ){
    Light client( user, r, g, b );
    clients.push_back( client );
}

//--------------------------------------------------------------
void testApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void testApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
	client.send("init-of");
}

//--------------------------------------------------------------
void testApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
	client.connect("macaroni.local", 8081);
}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){
    //cout<<"got message "<<args.message<<endl;
	
	vector <string> parts = ofSplitString(args.message, ",");
	
	if (parts[0] == "radius") {
		cout << "radius: " << parts[1] << endl;
		radius = ofToFloat(parts[1]);
		
	}
	if (parts[0] == "angle") {
		cout << "angle: " << parts[1] << endl;
		angle = ofToFloat(parts[1]);
	}
    
    if (parts[0] == "new-user") {
        cout << "new-user: " << parts[1] << " | (" << parts[2] << "," << parts[3] << "," << parts[4] << ")" << endl;
        newUser( parts[1], ofToInt(parts[2]), ofToInt(parts[3]), ofToInt(parts[4]) );
    }
    
    if (parts[0] == "orientation") {
        tiltLR = ofToFloat(parts[2]);
//        tiltLR = ofDegToRad(ofMap( tiltLR, -180, 180, 0, 360 ));
//        
        tiltFB = ofToFloat(parts[3]);
//        tiltFB = ofDegToRad(ofMap( tiltFB, -90, 90, 360, 0 ));
//        
//        dir = ofToFloat(parts[4]);
        
        for ( int i = 0; i < clients.size(); i++ ) {
            if ( parts[1] == clients[i].userId ) {
                clients[i].update( tiltLR, tiltFB );
            }
        }
    }
    
    if (parts[0] == "close"){
        for ( int i = 0; i < clients.size(); i++ ){
            if ( parts[1] == clients[i].userId ) {
                //clients.erase( clients.begin() + i );
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
//    client.send("Hello");
//	cout << "sending hello" <<endl;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}
