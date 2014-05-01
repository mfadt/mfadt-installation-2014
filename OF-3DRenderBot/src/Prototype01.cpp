//
//  Prototype01.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/23/13.
//
//
#include "Prototype01.h"

void Prototype01::selfSetup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //client.connect("localhost", 8081);
    //client.addListener(this);
    
    ofDirectory dir(getDataPath()+"objs/");
    int nDir = dir.listDir();
    for(int i = 0; i < nDir; i++){
        string name = dir.getName(i);
        names.push_back(name);
    }
    
    nCounter = 0;
    ofxAssimpModelLoader loader;
    loader.loadModel(getDataPath()+"objs/"+names[nCounter]+"/"+names[nCounter]+".obj", true);
    loader.update();
    nCounter++;
    meshTarget = loader.getMesh(0);
    deCompose(meshTarget, trianglesTarget);
    triangles = trianglesTarget;
    
    loader.loadModel(getDataPath()+"objs/"+names[nCounter]+"/"+names[nCounter]+".obj", true);
    loader.update();
    nCounter = (nCounter+1)%names.size();
    meshTarget = loader.getMesh(0);
    
    globalOffset.set(0,0,0);

    bNext = false;
}

void Prototype01::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    guiAdd(grid);
}

void Prototype01::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Prototype01::selfSetupSystemGui(){
    sysGui->addSlider("Speed",0.0,0.1,&speed);
    sysGui->addSlider("Rotation_Speed", 0.0, 0.01, &rotateSpeed);
    
    sysGui->addLabel("Target");
    sysGui->addSlider("attraction", 0.0, 1.0, &targetAttraction);
    sysGui->addSlider("attraction_radius", 0.01, 1.0, &targetRadius);
    sysGui->addSlider("traction", 0.0, 1.0, &targetTraction);
    
    sysGui->addLabel("Flocking");
    sysGui->addSlider("flocking_force", 0.0, 1.0, &flocking);
    sysGui->addSlider("turbulence", 0.0, 0.02, &turbulence);
    sysGui->addSlider("neigbordhood", 0.1, 1.1, &neigbordhood);
    sysGui->addSlider("independence", 0, 1.0, &independence);
}

//---------------------------------------------------

void Prototype01::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
}

void  Prototype01::deCompose(ofMesh &_mesh, vector<Triangle>& _triangles){
    
    modelOffset = _mesh.getCentroid();
    
    _triangles.clear();
    
    int nFaces = _mesh.getUniqueFaces().size();
    
    for (int i = 0; i < nFaces; i++) {
        Triangle t;
        t.a.set(_mesh.getUniqueFaces()[i].getVertex(0));
        t.b.set(_mesh.getUniqueFaces()[i].getVertex(1));
        t.c.set(_mesh.getUniqueFaces()[i].getVertex(2));
        t.normal.set(_mesh.getUniqueFaces()[i].getFaceNormal());
        
        t.set((t.a +t.b+t.c)/3);
        t.rot.makeRotate(180, t.normal );
        t.a.set(t.rot*(t.a-t));
        t.b.set(t.rot*(t.b-t));
        t.c.set(t.rot*(t.c-t));
        t.rot.inverse();
        
        float max = 0.07;
        if((t.a-t.b).length()<max && (t.a-t.c).length()<max && (t.b-t.c).length()<max){
            _triangles.push_back(t);
        }
    }
    
    return _triangles;
}

void Prototype01::selfUpdate(){
    
    if (bNext){
        deCompose(meshTarget,trianglesTarget);
        
        while (triangles.size() < trianglesTarget.size()) {
            triangles.push_back(triangles[ofRandom(triangles.size())]);
        }
        
        ofxAssimpModelLoader loader;
        loader.loadModel(getDataPath()+"objs/"+names[nCounter]+"/"+names[nCounter]+".obj", true);
        loader.update();
        nCounter = (nCounter+1)%names.size();
        meshTarget = loader.getMesh(0);
        bNext = false;
    }
    
    for (int i = triangles.size()-1; i >= 0 ; i--) {
        if(i<trianglesTarget.size()){
            
            if(targetAttraction>0.0)
                triangles[i].addAttractionForce(trianglesTarget[i], targetRadius, targetAttraction);
                
            if(targetTraction>0.0);
                triangles[i].addSlowdonForceTo(trianglesTarget[i], targetTraction,true);
            
            if(rotateSpeed>0.0)
                triangles[i].rotateTo(trianglesTarget[i],rotateSpeed);
            
            if(flocking>0.0)
                triangles[i].applyFlockingForce(&globalOffset,neigbordhood,independence,flocking);
            
            if(speed>0.0)
                triangles[i].update(speed);
            
        } else {
            triangles.erase(triangles.begin()+i);
        }
    }
    
    if(flocking>0.0)
        globalOffset += ofPoint(turbulence/neigbordhood, turbulence/neigbordhood, turbulence/neigbordhood);
}

void Prototype01::selfDraw(){
    materials["MATERIAL 1"]->begin();
    
    ofPushMatrix();
    ofTranslate(0,0,-100);
    ofRotate(90, 1, 0, 0);
    grid.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(modelOffset*-1.0);
    ofScale(300, 300,300);
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < triangles.size(); i++) {
        triangles[i].draw();
    }
    glEnd();
    
    ofPopMatrix();
    
    materials["MATERIAL 1"]->end();
}


void Prototype01::selfEnd(){
}

void Prototype01::selfExit(){
    
}

//--------------------------------------------------------------
void Prototype01::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void Prototype01::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
    client.send("init-of");
}

//--------------------------------------------------------------
void Prototype01::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    client.connect("localhost", 8080);
}

//--------------------------------------------------------------
void Prototype01::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void Prototype01::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
}

//--------------------------------------------------------------
void Prototype01::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

void Prototype01::selfKeyPressed(ofKeyEventArgs & args){
    if(args.key == ' '){
        bNext = true;
    }
}

void Prototype01::selfKeyReleased(ofKeyEventArgs & args){
	
}

void Prototype01::selfMouseMoved(ofMouseEventArgs& data){
	
}

void Prototype01::selfMousePressed(ofMouseEventArgs& data){

}

void Prototype01::selfMouseDragged(ofMouseEventArgs& data){

}

void Prototype01::selfMouseReleased(ofMouseEventArgs& data){

}