#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    client.connect("localhost", 8080);
    ofSetLogLevel(OF_LOG_ERROR);
    
    client.addListener(this);
	
	ofBackground(0);
	ofSetCircleResolution(60);
	ofNoFill();
	
	radius = 300;
	angle = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
		ofRotateX(angle);
		ofRotateY(ofGetElapsedTimef()*4);
		
		ofCircle(0, 0, radius);
	}
	ofPopMatrix();
	
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
	client.connect("localhost", 8080);
}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){
//    cout<<"got message "<<args.message<<endl;
	
	vector <string> parts = ofSplitString(args.message, ",");
	
	if (parts[0] == "radius") {
		cout << "radius: " << parts[1] << endl;
		radius = ofToFloat(parts[1]);
		
	}
	if (parts[0] == "angle") {
		cout << "angle: " << parts[1] << endl;
		angle = ofToFloat(parts[1]);
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
