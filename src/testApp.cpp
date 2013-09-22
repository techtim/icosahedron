#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(20);
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0, 0, 0);

	ofEnableAlphaBlending();    
//	ofSetVerticalSync(false);
    ofSetVerticalSync(true);
	// this uses depth information for occlusion rather than always drawing things on top of each other

    ofSetCircleResolution(64);

    setupCam(0);
    setupViewports();
    
    x_pos = 80;
	cam.getMouseInputEnabled();
//	shader.setGeometryInputType(GL_TRIANGLES);
//	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
//	shader.setGeometryOutputCount(4);
//	shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
    //
    //	printf("Maximum number of output vertices support is: %i\n", shader.getGeometryMaxOutputCount());
    
    // -- GUI --
    gui.setup();
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
	gui.add(filled.set("bFill", true));
	gui.add(radius.set( "radius", 140, 10, 300 ));
	gui.add(center.set("center",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
	gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
	gui.add(circleResolution.set("circleRes", 5, 3, 90));
	gui.add(twoCircles.setup("twoCircles"));
	gui.add(ringButton.setup("ring"));
	gui.add(screenSize.set("screenSize", ""));
    
    icoNormals[0] = ofVec3f(0.000f,  0.000f,  1.000f); // 0
    icoNormals[1] = ofVec3f(-0.724f,  0.526f,  0.447f); // 3
    icoNormals[2] = ofVec3f(-0.724f, -0.526f,  0.447f); // 4

    
    font.loadFont("type/verdana.ttf", 100, true, false, true, 0.4, 72);
	doShader = false;
    
    x_pos=0, y_pos=60;
    
    int i, j = 0;
    for ( i = 0; i < 12; i++ )
    {
        
        c[i].r = ofRandom(1.0);
        c[i].g = ofRandom(1.0);
        c[i].b = ofRandom(1.0);
        
        v[i][0] = Verts[j] * 100.f;
        j++;
        v[i][1] = Verts[j] * 100.f;
        j++;
        v[i][2] = Verts[j] * 100.f;
        j++;
        
    }
    
//    vbo.setVertexData( &icoVerts[0], 12, GL_STATIC_DRAW );
//    vbo.setColorData( &c[0], 12, GL_STATIC_DRAW );
//    vbo.setIndexData( &Faces[0], 60, GL_STATIC_DRAW );
    
    icoMesh.addVertices(&icoVerts[0], 12);
    icoMesh.addIndices(&Faces[0], 60);
//    icoMesh.addIndices(&icoIndices);

    boxMesh.addVertices(&boxVertices[0], 8);
//	boxMesh.addNormals(boxNormals,8);
	boxMesh.addIndices(boxIndices, 3*2*6);
//    boxMesh = boxMesh.box(1.0f, 0.8f, 0.8f);
    
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    
    // --- OUTPUT ---
    udpConnection.Create();
	udpAvailable = udpConnection.Connect(RPI_HOST, RPI_PORT);
	udpConnection.SetNonBlocking(true);
    udpAvailable = false;
}

// -- CAMERA --
void testApp::setupCam(int main_cam_num){

    for (int i=0; i<N_CAMERAS; i++) {
//        cameras[i] = new ofEasyCam();
//        cameras[i]->setupPerspective(true);
        cameras[i] = new orthoCamera();
//        cameras[i]->setNearClip(2.9f);
//        cameras[i]->setFarClip(3.1f);
        cameras[i]->setNearClip(.75f);
        cameras[i]->setFarClip(1.f);

        //    cameras[i](0.0f, 0.0f, 5.0f);
        cameras[i]->setPosition(icoVerts[Faces[i*3+0]]+icoVerts[Faces[i*3+1]]+icoVerts[Faces[i*3+2]]);
        cameras[i]->lookAt(ofVec3f(0.0f,  0.0f,  0.0f));
        cameras[i]->dolly(2.f);
//        cameras[i]->setDistance(1.75f);
    }
    cam.setupPerspective(true);
    
    cam.setNearClip(3.0f);
    cam.setFarClip(5.1f);
    cam.setPosition(0.755f, -2.64f, -0.5f);
    selectCam = 0;
//    cam.setPosition(icoVerts[Faces[selectCam*3+0]]+icoVerts[Faces[selectCam*3+1]]+icoVerts[Faces[selectCam*3+2]]);
    // 1.907f, 1.387f, 3.087f
    cam.lookAt(ofVec3f(0.0f,  0.0f,  0.0f));
//    cam.setDistance(3.89f);
    cam.setDistance(2.f);
    x_pos = 80;
	cam.getMouseInputEnabled();
}

void testApp::setupViewports(){
	float xOffset = ofGetWidth() / 2;
	float yOffset = ofGetHeight() / (N_CAMERAS / 2);
    
	viewMain.x = xOffset;
	viewMain.y = 0;
	viewMain.width = xOffset;
	viewMain.height = ofGetHeight();
    
	for(int i = 0; i < N_CAMERAS; i++){
        if (i < N_CAMERAS/2) {
            viewGrid[i].x = 0;
            viewGrid[i].y = yOffset * i;
            viewGrid[i].width = xOffset/2;
            viewGrid[i].height = yOffset;
        } else {
            viewGrid[i].x = xOffset/2;
            viewGrid[i].y = yOffset * (i-N_CAMERAS/2);
            viewGrid[i].width = xOffset/2;
            viewGrid[i].height = yOffset;
        }
	}
}

//--------------------------------------------------------------
void testApp::update(){
    cam.setDistance(x_pos);
}

//--------------------------------------------------------------
void testApp::draw(){

    ofEnableDepthTest();
//    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    cam.begin(viewMain);
    
//
    doShader ? drawFirstInSecond() : drawScene(true);

//    drawScene(false);
//        ofSetColor(255, 255, 255);
    for (int i=0; i<12; i++){
		// we do a manual scale by scaling the mesh vertex coord by 50.f to render the point at the vertex position
        glDisable(GL_CULL_FACE);
		ofDrawBitmapString(ofToString(i), icoVerts[i]);
	}
    
    cam.end();
    screenImg.grabScreen(viewMain.x, viewMain.y, viewMain.width, viewMain.height);
    drawToUdp(screenImg);

    for(int i = 0; i < N_CAMERAS; i++){
		ofSetColor(255, 255, 255);
        ofNoFill();
        ofRect(viewGrid[i]);
        
        cameras[i]->begin(viewGrid[i]);
       
        ofVec3f e1 = icoVerts[Faces[i*3+0]] - icoVerts[Faces[i*3+1]];
        ofVec3f e2 = icoVerts[Faces[i*3+2]] - icoVerts[Faces[i*3+1]];
        ofVec3f no = e2.cross( e1 );

        GLdouble eq[4] = {no[0], no[1], no[2], 1.51f};
//        glEnable(GL_CLIP_PLANE0);
//        glClipPlane(GL_CLIP_PLANE0, eq);
        
		drawScene(false);
//        glDisable(GL_CLIP_PLANE0);
        ofFill();
        ofSetColor(200, 200, 200);

        for (int j=0; j<12; j++){
            // we do a manual scale by scaling the mesh vertex coord by 50.f to render the point at the vertex position
            ofDrawBitmapString(ofToString(j), icoVerts[j]);
            
        }
        ofNoFill();
		cameras[i]->end();
	}
	//

    ofDisableDepthTest();
//    glDepthFunc(GL_ALWAYS);
    ofSetColor(255.f, 255.f, 255.f);
    ofDrawBitmapString("X="+ofToString(cam.getX()), 20.0f, 10.0f);
    ofDrawBitmapString("Y="+ofToString(cam.getY()), 100.5f, 10.0f);
    ofDrawBitmapString("Z="+ofToString(cam.getZ()), 200.0f, 10.0f);
    ofDrawBitmapString("FPS="+ofToString(ofGetFrameRate()), 300.0f, 10.0f);
    ofDrawBitmapString("DIST="+ofToString(cam.getDistance()), 11.5f, 40.0f);
    ofDrawBitmapString("  POS="+ofToString(cam.getPosition()), 150.5f, 40.0f);
    ofVec3f tmp = icoVerts[Faces[selectCam*3+0]]+icoVerts[Faces[selectCam*3+1]]+icoVerts[Faces[selectCam*3+2]];
    ofDrawBitmapString("O_POS="+ofToString(tmp),150.5f, 50.0f);
    
    ofDrawBitmapString(ofToString(cam.getLocalTransformMatrix()), 521.5f, 60.0f);
//	getX()
//    getY()
//    getZ()
//	getFarClip()
//	ofVec2f getLensOffset()

    gui.draw();
//    glDepthFunc(GL_LESS);
}

void testApp::drawScene(bool is_main) {

//    ofSetBackgroundColor(10, 10, 10);
    glEnable(GL_DEPTH_TEST);
    ofSetColor(0, 200, 255);
    
    ofNoFill();

    glLineWidth(2.0f);
    //    glCullFace(GL_FRONT);
    
    icoMesh.draw(OF_MESH_WIREFRAME);
ofFill();
//    ofSpere(0, 0, 0, 2.5f*sin(ofGetFrameNum()*.01f), 2.5f*sin(ofGetFrameNum()*.01f));
    ofSetColor(0, 255.0f * sin(ofGetFrameNum()*1.0f), 100, 150);
//    ofSpherePrimitive(2.5f*sin(ofGetFrameNum()*.01f), 60);
//    ofDrawSphere( 1.5f*sin(ofGetFrameNum()*.07f));
    ofDrawSphere(1.0f*sin(ofGetFrameNum()*.07f), 1.f*cos(ofGetFrameNum()*.07f), .0f,  .7f*sin(ofGetFrameNum()*.02f));
//    boxMesh.box( 1.2f*sin(ofGetFrameNum()*.1f) );
//
//    ofxCarveCSG booleanOperator;
//    resultCsg = booleanOperator.meshSetToOfMesh(booleanOperator.booleanOperation(ofxCarveCSG::INTERSECTION, booleanOperator.ofMeshToMeshSet(icoMesh), booleanOperator.ofMeshToMeshSet(boxMesh)));
//    boxMesh = boxMesh.xbox(1.5f*cos(ofGetFrameNum()*.06f), 1.5f, 1.5f);
//    boxMesh.drawInMesh(icoMesh);
//    resultCsg.draw();
//    ofDrawBox(1.2f*sin(ofGetFrameNum()*.1f));
    
    ofSetColor(200.0f*sin(ofGetFrameNum()*.01f), 200, 0, 100);
    ofDrawCone(0, 0, 0, 1.0f*sin(ofGetFrameNum()*.01f), 1.5f*sin(ofGetFrameNum()*.07f));
    
//    ofPushMatrix();
//        //    ofTranslate(0, 0, 0);
//        ofTranslate(0, 1-2*sin(ofGetFrameNum()*.05f), 0);
//        boxMesh.draw();
//    ofPopMatrix;
    
    ofSetColor(150, 0, 200.0f*sin(ofGetFrameNum()*.01f), 200);
    ofDrawSphere(0.0f, .0f, 0.0f, 1.f*cos(ofGetFrameNum()*0.05f));
    
//    glDonut(0, 0, 50, 5, 0.0f, 0.0f, 1.0f, 0.3f);

//    ofDrawPlane(0, 0, 0, 2.0f*sin(ofGetFrameNum()*.01f), 2.0f*sin(ofGetFrameNum()*.01f));
//
    
}

void testApp::drawToUdp(ofImage img) {
    if (udpAvailable) {
        unsigned char * pix = img.getPixels();
        char to_leds [N_LEDS*3];
        for (int i = 0; i<N_LEDS; i++ ) {
            //        leds[i] = screenImg.getColor(ofGetWidth()/2, 200+i);
            //        memccpy((to_leds[i*3+0], (char)pix[ofGetWidth()/2*(200+i)+0]
            ofColor cl = img.getColor(ofGetWidth()/2+viewMain.width/2, ofGetHeight()/2-N_LEDS/2+i);
            //        to_leds[i*3+0] = (char)pix[ofGetWidth()/2*(200+i)+0];
            //        to_leds[i*3+1] = (char)pix[ofGetWidth()/2*(200+i)+1];
            //        to_leds[i*3+2] = (char)pix[ofGetWidth()/2*(200+i)+2];
            to_leds[i*3+0] = (char)cl.r;
            to_leds[i*3+1] = (char)cl.b;
            to_leds[i*3+2] = (char)cl.g;
    //        printf("%02X %02X %02X \n", to_leds[i*3+0], to_leds[i*3+1], to_leds[i*3+2]);
        }
    //    printf("\n \n");
        udpConnection.Send(to_leds, N_LEDS*3);
    //    ofLine(ofGetWidth()/2+viewMain.width/2, ofGetHeight()/2-N_LEDS/2, ofGetWidth()/2+viewMain.width/2, ofGetHeight()/2+N_LEDS/2);
    } else {
//        printf("Cant send UDP message \n");
    }
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if( key == 's' ){
		doShader = !doShader;
	}
    
    if (key == OF_KEY_CONTROL) {
        
    }
    
    if( key == OF_KEY_RIGHT) {
        x_pos+=1;
    }
    if( key == OF_KEY_LEFT) {
        x_pos-=1;
    }
    if( key == OF_KEY_DOWN) {
        y_pos+=1;
    }
    if( key == OF_KEY_UP) {
        y_pos-=1;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    mouseX = x;
	mouseY = y;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}

// void testApp::drawFirstInSecond ( void (*first)(), void (*second)() )
void testApp::drawFirstInSecond ( )
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glEnable      ( GL_STENCIL_TEST );
    
    glStencilFunc ( GL_ALWAYS, 1, 0 );
    glStencilOp   ( GL_KEEP, GL_KEEP, GL_REPLACE );
    
    drawScene(false);
    
    glStencilFunc ( GL_ALWAYS, 2, 0 );
    glStencilOp   ( GL_KEEP, GL_KEEP, GL_REPLACE );
    
    icoMesh.draw();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glStencilFunc(GL_EQUAL, 1, 255);
    
    icoMesh.draw();
    
    glDisable     ( GL_STENCIL_TEST );
}

void testApp::drawInIco() {
    glEnable    ( GL_CULL_FACE );
    glEnable    ( GL_DEPTH_TEST );
    glColorMask ( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    glCullFace  ( GL_BACK );
    
    icoMesh.draw();
//    first.draw();
    
    
    glDepthMask   ( GL_FALSE );
    glEnable      ( GL_STENCIL_TEST );
    glStencilOp   ( GL_KEEP, GL_KEEP, GL_INCR );
    glStencilFunc ( GL_ALWAYS, 0, 0 );
    
//    second.draw();\
    ofDrawSphere(sin(ofGetFrameNum()*0.01)*1.5f);
//    boxMesh = ofMesh::box(sin(ofGetFrameNum()*0.01)*1.5f, cos(ofGetFrameNum()*0.01)*1.5f, .5f);
//    boxMesh.draw();
    drawScene(0);
    
    glStencilOp ( GL_KEEP, GL_KEEP, GL_DECR );
    glCullFace  ( GL_FRONT );
    
//    second.draw();
    boxMesh.draw();
//    drawScene(0);
    
    glDepthMask   ( GL_TRUE );
    glColorMask   ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glStencilFunc ( GL_NOTEQUAL, 0, 1 );
    glDisable     ( GL_DEPTH_TEST );
    glCullFace    ( GL_BACK );

    icoMesh.draw();
    
    glDisable     ( GL_STENCIL_TEST );
    
}


void testApp::fixDepth ()
{
//    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glColorMask ( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    glEnable    ( GL_DEPTH_TEST );
    glDisable   ( GL_STENCIL_TEST );
    glDepthFunc ( GL_ALWAYS );
    
//    draw ();
    icoMesh.draw();
    
    glDepthFunc ( GL_LESS );
}
