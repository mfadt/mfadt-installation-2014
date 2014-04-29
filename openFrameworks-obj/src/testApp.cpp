#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetSmoothLighting(true);
    ofEnableDepthTest();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    client.connect("localhost", 8081);
    ofSetLogLevel(OF_LOG_ERROR);
    
    client.addListener(this);
	
	ofBackground(0);
	ofSetCircleResolution(60);
	ofNoFill();
    
    //  Easy Cam
    //
    cam.setDistance(500);
    
    //  Model
    //
    ofxAssimpModelLoader loader;
    loader.loadModel("objs/Asega_salome/Asega_salome.obj", true);
    loader.update();
    mesh = loader.getMesh(0);
    texture = loader.getTextureForMesh(0);
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofDisableLighting();
    ofSetColor(255);
    
    ofEnableLighting();
	ofPushMatrix();
	{
        cam.begin();
        
        for (std::map<string,Light>::iterator it=lights.begin(); it!=lights.end(); ++it){
            it->second.enable();
        }
        
        ofScale(300, 300,300);
        texture.bind();
        mesh.draw();
        texture.unbind();

        cam.end();

	}
	ofPopMatrix();
    ofDisableLighting();
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
	client.connect("localhost", 8081);
}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){

	vector<string> parts = ofSplitString(args.message, ",");
    
    if(parts.size() > 1){
        string id = parts[1];
        if (parts[0] == "radius") {
            lights[id].radius = ofToFloat(parts[1]);
        } else if (parts[0] == "new-user") {
            lights[id].color.set(ofToInt(parts[2]), ofToInt(parts[3]), ofToInt(parts[4]));
        } else if (parts[0] == "orientation") {
            lights[id].tiltLR = ofToFloat(parts[2]);
            lights[id].tiltFB = ofToFloat(parts[3]);
        } else if (parts[0] == "close"){
            lights.erase(id);
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
