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
    meshLoader.loadModel(getDataPath()+"objs/"+names[nCounter]+"/"+names[nCounter]+".obj", true);
    meshLoader.update();
    meshTarget = meshLoader.getMesh(0);
    meshLoader.getSceneCenter();

    nCounter++;
    nFaceCounter = 0;

    globalOffset.set(0,0,0);

    bNext = false;
}

void Prototype01::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    materialAdd("FONT_MAT");
    guiAdd(grid);
}

void Prototype01::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Prototype01::selfSetupSystemGui(){
    
    sysGui->addIntSlider("Faces_for_Frame", 0, 1000, &nFaceForFrame);
    sysGui->addSlider("Explotion", 0.0, 1.0, &explotion);
    
    sysGui->addLabel("Physics");
    sysGui->addSlider("Speed",0.0,0.2,&speed);
    sysGui->addSlider("Rotation_Speed", 0.0, 0.01, &rotateSpeed);
    
    sysGui->addLabel("Target");
    sysGui->addSlider("attraction", 0.0, 1.0, &targetAttraction);
    sysGui->addSlider("attraction_radius", 0.01, 1.0, &targetRadius);
    sysGui->addSlider("traction", 0.0, 1.0, &targetTraction);
    
    sysGui->addLabel("Flocking");
    sysGui->addSlider("flocking_force", 0.0, 0.1, &flocking);
    sysGui->addSlider("turbulence", 0.0, 0.02, &turbulence);
    sysGui->addSlider("neigbordhood", 0.1, 1.1, &neigbordhood);
    sysGui->addSlider("independence", 0, 1.0, &independence);
}

void Prototype01::selfSetupRenderGui(){
    rdrGui->addLabel("Name_Font");
    rdrGui->addSlider("Name_Font_Size", 0, 60, &fontNameSize);
    rdrGui->addSlider("Name_Font_Deep", 0, 20, &fontNameDeep);
    rdrGui->addSlider("Name-Font_Alpha", 0, 1, &fontNameAlpha);
    
    
}

void Prototype01::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
}

void Prototype01::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name.find("Name_Font") == 0){
        font.loadFont(getDataPath()+"mplus-1c-regular.ttf", fontNameSize, fontNameDeep);
    }
}

//---------------------------------------------------

void Prototype01::selfBegin(){
    font.loadFont(getDataPath()+"mplus-1c-regular.ttf", fontNameSize, fontNameDeep);
}

void Prototype01::selfUpdate(){
    
    if (bNext){
        meshLoader.loadModel(getDataPath()+"objs/"+names[nCounter]+"/"+names[nCounter]+".obj", true);
        meshLoader.update();
        nCounter = (nCounter+1)%names.size();
        meshTarget = meshLoader.getMesh(0);
        meshOffset = meshLoader.getSceneCenter();
        nFaceCounter = 0;
        bNext = false;
    }
    
    for(int i = 0; i < nFaceForFrame; i++){
        
        //  Add if is need
        //
        if (nFaceCounter < meshTarget.getUniqueFaces().size()){
            Triangle t;
            t.a.set(meshTarget.getUniqueFaces()[nFaceCounter].getVertex(0)-meshOffset);
            t.b.set(meshTarget.getUniqueFaces()[nFaceCounter].getVertex(1)-meshOffset);
            t.c.set(meshTarget.getUniqueFaces()[nFaceCounter].getVertex(2)-meshOffset);
            t.normal.set(meshTarget.getUniqueFaces()[nFaceCounter].getFaceNormal());
            
            
            t.set((t.a+t.b+t.c)/3);
            t -= meshOffset*explotion;
            t.rot.makeRotate(180, t.normal);
            
            ofQuaternion inv = t.rot;
            inv.inverse();
            t.a.set(inv*(t.a-t));
            t.b.set(inv*(t.b-t));
            t.c.set(inv*(t.c-t));
            t.original_normal.set(inv*t.normal);
            
            float max = 0.07;
            if((t.a-t.b).length()>=max ||
               (t.a-t.c).length()>=max ||
               (t.b-t.c).length()>=max ){
                t.a.set(0,0,0);
                t.b.set(0,0,0);
                t.c.set(0,0,0);
            }
            
            if(nFaceCounter<trianglesTarget.size()){
                trianglesTarget[nFaceCounter] = t;
            } else {
                trianglesTarget.push_back(t);
            }
            nFaceCounter++;
        }
        
        if (trianglesTarget.size() > meshTarget.getUniqueFaces().size()){
            trianglesTarget.erase(trianglesTarget.end());
        }
        
        if (triangles.size() > trianglesTarget.size()){
            triangles.erase(triangles.end());
        }
    }
    
    //  Add extra Triangles
    //
    while (triangles.size() < trianglesTarget.size()) {
        Triangle t = trianglesTarget[ofRandom(trianglesTarget.size())];
        t.set(ofRandom(-2.,2.), ofRandom(-2.,2.), ofRandom(-2.,2.));
        triangles.push_back(t);
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
    
    ofScale(300,300,300);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < triangles.size(); i++) {
        triangles[i].draw();
    }
    glEnd();
    
    if(bDebug){
        glBegin(GL_LINES);
        ofPushStyle();
        ofSetColor(255, 0, 0);
        for (int i = 0; i < triangles.size(); i++) {
            glVertex3f(triangles[i].x, triangles[i].y, triangles[i].z);
            glVertex3f((triangles[i]+triangles[i].normal*0.01).x,
                       (triangles[i]+triangles[i].normal*0.01).y,
                       (triangles[i]+triangles[i].normal*0.01).z);
        }
        ofPopStyle();
        glEnd();
        
        glBegin(GL_LINES);
        ofPushStyle();
        ofSetColor(0, 0, 255);
        for (int i = 0; i < triangles.size(); i++) {
            glVertex3f(triangles[i].x, triangles[i].y, triangles[i].z);
            glVertex3f((triangles[i]+triangles[i].original_normal*0.01).x,
                       (triangles[i]+triangles[i].original_normal*0.01).y,
                       (triangles[i]+triangles[i].original_normal*0.01).z);
        }
        ofPopStyle();
        glEnd();
    }
    
    ofPopMatrix();
    
    materials["MATERIAL 1"]->end();
    
    materials["FONT_MAT"]->begin();
    ofPushMatrix();
    ofRotate(90, 1, 0, 0);
    ofRotate(90, 0, 1, 0);
    ofTranslate(0, -50, 50);
    ofScale(1, -1, 1);  // Flip back since we're in 3D.
    ofSetColor(255,255*fontNameAlpha);
    font.drawString(names[nCounter-1], font.stringWidth(names[nCounter-1]) * -0.5f, font.stringHeight(names[nCounter-1]) * 0.5f);
    ofPopMatrix();
    materials["FONT_MAT"]->end();
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
        
        UIMapBackground *back = (UIMapBackground*)background;
        lights["SPOT"]->diffuse.hue = ofRandom(1.0);
        lights["POINT LIGHT 1"]->diffuse.hue = ofRandom(1.0);
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