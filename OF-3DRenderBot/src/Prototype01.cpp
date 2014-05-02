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
    
    terrainShader.load(getDataPath()+"shaders/terrain");
    
    ofDirectory dir(getDataPath()+"objs/");
    int nDir = dir.listDir();
    for(int i = 0; i < nDir; i++){
        string name = dir.getName(i);
        names.push_back(name);
    }
    
    bNext = true;
    nCounter = 0;
    nFaceCounter = 0;

    globalOffset.set(0,0,0);
}

void Prototype01::selfSetupGuis(){
    backgroundSet(new UIMapBackground());
    lightAdd("SPOT", OF_LIGHT_SPOT);
    materialAdd("FONT_MAT");
    guiAdd(grid);
    guiAdd(terrainShader);
}

void Prototype01::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Prototype01::selfSetupSystemGui(){
    sysGui->addIntSlider("Faces_for_Frame", 0, 1000, &nFaceForFrame);
    
    sysGui->addLabel("Physics");
    sysGui->addSlider("Speed",0.0,1.0,&speed);
    sysGui->addSlider("Rotation_Speed", 0.0, 1.0, &rotateSpeed);
    
    sysGui->addLabel("Target");
    sysGui->addSlider("attraction", 0.0, 1.0, &targetAttraction);
    sysGui->addSlider("attraction_radius", 0.01, 10.0, &targetRadius);
    sysGui->addSlider("traction", 0.0, 1.0, &targetTraction);
    
    sysGui->addLabel("Flocking");
    sysGui->addSlider("flocking_force", 0.0, 1.0, &flocking);
    sysGui->addSlider("turbulence", 0.0, 1.0, &turbulence);
    sysGui->addSlider("neigbordhood", 0.1, 100.1, &neigbordhood);
    sysGui->addSlider("independence", 0, 1.0, &independence);
    
    sysGui->addLabel("BackTerrain");
    sysGui->addIntSlider("terrain_width", 1, 10000, &terrainWidth);
    sysGui->addIntSlider("terrain_height",1, 10000, &terrainHeight);
    sysGui->addSlider("terrain_scale",10,200,&terrainScale);
    sysGui->addSlider("terrain_wireframe_alpha", 0, 1, &terrainWireframeAlpha);
}

void Prototype01::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name.find("terrain_") == 0){
        terrainMake();
    }
}

void Prototype01::selfSetupRenderGui(){
    
    rdrGui->addSlider("Texture_Alpha", 0.0, 1.0, &meshTextureAlpha);
    
    rdrGui->addLabel("Name_Font");
    rdrGui->addSlider("Name_Font_Size", 0, 60, &fontNameSize);
    rdrGui->addSlider("Name_Font_Deep", 0, 20, &fontNameDeep);
    rdrGui->addSlider("Name-Font_Alpha", 0, 1, &fontNameAlpha);
}

void Prototype01::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name.find("Name_Font") == 0){
        font.loadFont(getDataPath()+"Exo2-Light.ttf", fontNameSize, fontNameDeep);
    }
}

//---------------------------------------------------

void Prototype01::selfBegin(){
    font.loadFont(getDataPath()+"Exo2-Light.ttf", fontNameSize, fontNameDeep);
}

void Prototype01::terrainMake(){
    
    terrain.clear();
    
    int w = terrainWidth/terrainScale;
    int h = terrainHeight/terrainScale;
	for (int y = 0; y < h; y++){
		for (int x = 0; x<w; x++){
            float offsetX = 0;
            float offsetY = (x%2==1)?0.5:0.0;
			terrain.addVertex(ofPoint((x+offsetX)*terrainScale,(y+offsetY)*terrainScale,0));
            terrain.addNormal(ofPoint(1,0,0));
            terrain.addTexCoord(ofVec2f((x+offsetX)*terrainScale,(y+offsetY)*terrainScale));
		}
	}
	for (int y = 0; y<h-1; y++){
		for (int x=0; x<w-1; x++){
            if(x%2==0){
                terrain.addIndex(x+y*w);				// a
                terrain.addIndex((x+1)+y*w);			// b
                terrain.addIndex(x+(y+1)*w);			// d
                
                terrain.addIndex((x+1)+y*w);			// b
                terrain.addIndex((x+1)+(y+1)*w);		// c
                terrain.addIndex(x+(y+1)*w);			// d
            } else {
                terrain.addIndex((x+1)+y*w);			// b
                terrain.addIndex(x+y*w);				// a
                terrain.addIndex((x+1)+(y+1)*w);		// c
                
                terrain.addIndex(x+y*w);				// a
                terrain.addIndex(x+(y+1)*w);			// d
                terrain.addIndex((x+1)+(y+1)*w);		// c
            }
		}
	}
}

void Prototype01::selfUpdate(){
    
    if (bNext){
        textName = names[nCounter];
        meshLoader.enableTextures();
        meshLoader.enableMaterials();
        meshLoader.loadModel(getDataPath()+"objs/"+names[nCounter]+"/TEXTURED_"+names[nCounter]+".obj", true);
        ofDisableArbTex();
        ofLoadImage(meshTexture, getDataPath()+"objs/"+names[nCounter]+"/color.png");
        meshDimTexture.allocate(meshTexture.getWidth(), meshTexture.getHeight());
        ofEnableArbTex();
        
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
            
            t.tA.set(meshTarget.getUniqueFaces()[nFaceCounter].getTexCoord(0));
            t.tB.set(meshTarget.getUniqueFaces()[nFaceCounter].getTexCoord(1));
            t.tC.set(meshTarget.getUniqueFaces()[nFaceCounter].getTexCoord(2));
        
            t.set((t.a+t.b+t.c)/3);
            t.rot.makeRotate(180, t.normal);
            
            ofQuaternion inv = t.rot;
            inv.inverse();
            t.a.set(inv*(t.a-t));
            t.b.set(inv*(t.b-t));
            t.c.set(inv*(t.c-t));
            t.original_normal.set(inv*t.normal);

            float max = 7.0;
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
        t.set( ofPoint(ofRandom(-1.,1.), ofRandom(-1.,1.), ofRandom(-1.,1.)));
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
    
    ofPushStyle();
    ofDisableArbTex();
    meshDimTexture.begin();
    ofClear(255,255);
    ofSetColor(255.0,255.0*meshTextureAlpha);
    meshTexture.draw(0,0);
    ofEnableAlphaBlending();
    meshDimTexture.end();
    ofEnableArbTex();
    ofPopStyle();
}

void Prototype01::selfDraw(){
    materials["MATERIAL 1"]->begin();
    
    UIMapBackground *back = (UIMapBackground*)background;
    ofPushMatrix();
    ofTranslate(-terrainWidth*0.5, -terrainHeight*0.5,-200);
    terrainShader.begin();
    terrainShader.getShader().setUniform3f("color1", back->color.r, back->color.g, back->color.b );
    terrainShader.getShader().setUniform3f("color2", back->color2.r, back->color2.g, back->color2.b );
    terrain.drawFaces();
    terrainShader.end();

    if(terrainWireframeAlpha>0.0){
        ofTranslate(0,0,1);
        terrainShader.begin();
        terrainShader.getShader().setUniform3f("color1", back->color2.r, back->color2.g, back->color2.b );
        terrainShader.getShader().setUniform3f("color2", back->color2.r, back->color2.g, back->color2.b );
        terrainShader.getShader().setUniform1f("AlphaPct", terrainWireframeAlpha);
        terrain.drawWireframe();
        terrainShader.end();
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0,0,-50);
    ofRotate(90, 1, 0, 0);
    grid.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofScale(3,3,3);
    
    ofPushMatrix();
    ofDisableArbTex();
    meshDimTexture.getTextureReference().bind();
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < triangles.size(); i++) {
        triangles[i].draw();
    }
    glEnd();
    meshDimTexture.getTextureReference().unbind();
    ofEnableArbTex();

    if(bDebug){
        glBegin(GL_LINES);
        ofPushStyle();
        ofSetColor(255, 0, 0);
        for (int i = 0; i < triangles.size(); i++) {
            glVertex3f(triangles[i].x, triangles[i].y, triangles[i].z);
            glVertex3f((triangles[i]+triangles[i].normal).x,
                       (triangles[i]+triangles[i].normal).y,
                       (triangles[i]+triangles[i].normal).z);
        }
        ofPopStyle();
        glEnd();
        
        glBegin(GL_LINES);
        ofPushStyle();
        ofSetColor(0, 0, 255);
        for (int i = 0; i < triangles.size(); i++) {
            glVertex3f(triangles[i].x, triangles[i].y, triangles[i].z);
            glVertex3f((triangles[i]+triangles[i].original_normal).x,
                       (triangles[i]+triangles[i].original_normal).y,
                       (triangles[i]+triangles[i].original_normal).z);
        }
        ofPopStyle();
        glEnd();
    }
    ofPopMatrix();
    materials["MATERIAL 1"]->end();
    
    materials["FONT_MAT"]->begin();
    ofPushMatrix();
    ofTranslate(0, -20, 20);
    ofScale(1, -1, 1);  // Flip back since we're in 3D.
    ofSetColor(255,255*fontNameAlpha);
    font.drawString(textName, font.stringWidth(textName) * -0.5f, font.stringHeight(textName) * 0.5f);
    ofPopMatrix();
    materials["FONT_MAT"]->end();
    
    ofPopMatrix();
}

void Prototype01::selfDrawOverlay(){
    
    //  MAT: here you draw in 2D like a regular app in case you want to add overall information/images/etc
    //
//    ofDrawBitmapString("Hi there!", ofGetWidth()*0.5,ofGetHeight()*0.5);
    
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
    cout<<"on close"<<endl;}

//--------------------------------------------------------------
void Prototype01::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void Prototype01::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    vector<string> values = ofSplitString(args.message, " ");
    if (values[0] == "flocking"){
        flocking = ofToFloat(values[1]);
    } else if (values[0] == "text"){
        textName = values[1];
    } else if (values[0] == "load_model"){
        meshLoader.enableTextures();
        meshLoader.enableMaterials();
        meshLoader.loadModel(getDataPath()+"objs/"+values[1]+"/TEXTURED_"+values[1]+".obj", true);
        ofDisableArbTex();
        ofLoadImage(meshTexture, getDataPath()+"objs/"+values[1]+"/color.png");
        meshDimTexture.allocate(meshTexture.getWidth(), meshTexture.getHeight());
        ofEnableArbTex();
        meshTarget = meshLoader.getMesh(0);
        meshOffset = meshLoader.getSceneCenter();
        nFaceCounter = 0;
    } else if (values[0] == "texture_alpha"){
        meshTextureAlpha = ofToFloat(values[1]);
    } else if (values[0] == "speed"){
        speed = ofToFloat(values[1]);
    } else if (values[0] == "speed"){
        speed = ofToFloat(values[1]);
    } else if (values[0] == "camera"){
        //  Load camera like FAR, FRONT, RIGHT and UNDER
        //  You can make more going to the pannel that say "EASYCAM" and typing new names
        //  To transit between cameras position be sure to have the lerp in a low number.
        //
        camera->load(getDataPath()+"cameras/"+values[1]+".cam");
    }
    
    //  Etc etc
    //
    //  You get the picture here
    //
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