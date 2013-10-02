#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0, 0, 0);

	ofEnableAlphaBlending();    
//	ofSetVerticalSync(false);
    ofSetVerticalSync(true);
	// this uses depth information for occlusion rather than always drawing things on top of each other

    ofSetCircleResolution(64);

    setupViewports();
    setupCam(0);

    
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
	gui.add(drawSides.setup("draw Sides",false));
	gui.add(drawKinect.setup("draw Kinect", true));
	gui.add(screenSize.set("screenSize", ""));
    gui.add(coeff.set("coef", 0.5, 0, 1));
    gui.add(pos.set("pos", ofVec3f(0,0,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    
    font.loadFont("type/verdana.ttf", 100, true, false, true, 0.4, 72);
	doShader = false;
    
    x_pos=0, y_pos=60;
        
    icoMesh = icoUtils::get_vertices();

    boxMesh.addVertices(&boxVertices[0], 8);
//	boxMesh.addNormals(boxNormals,8);
	boxMesh.addIndices(boxIndices, 3*2*6);
//    boxMesh = boxMesh.box(1.0f, 0.8f, 0.8f);
    boxMesh.box(0.2f, 1.5f, 1.5f);

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    
    // --- KINECT ---
//    kinect.setRegistration(true);

	//kinect.init(true); // shows infrared instead of RGB video image
//    kinect.init(true, false); // disable video image (faster fps)
	
//	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}

    kinectMesh.setMode(OF_PRIMITIVE_POINTS);
    
    kinectMesh.addVertices(kinectVerts, KINECT_W*KINECT_H);
//	grayImage.allocate(kinect.width, kinect.height);

    // --- OUTPUT ---
    udpConnection.Create();
	udpAvailable = udpConnection.Connect(RPI_HOST2, RPI_PORT);
	udpConnection.SetNonBlocking(true);
    udpAvailable = true;
}

// --------- CAMERAS SETUP --------------

void testApp::setupCam(int main_cam_num){
    // setup points for each cam pos to grab triangles sides
    triGrabPoints = new ofVec3f*[N_CAMERAS];

    for (int i=0; i<N_CAMERAS; i++) {
//        cameras[i] = new ofEasyCam();
//        cameras[i] = new orthoCamera();
        cameras[i] = new ofCamera();
        cameras[i]->setupPerspective(false);
//        cameras[i]->setFov(0.f);
//        cameras[i]->enableOrtho();


        cameras[i]->setPosition(
            icoVerts[icoUniqSides[i*3+0]]+icoVerts[icoUniqSides[i*3+1]]+icoVerts[icoUniqSides[i*3+2]]
        );
//        cameras[i]->setPosition(icoVerts[Faces[i*3+0]]+icoVerts[Faces[i*3+1]]+icoVerts[Faces[i*3+2]]);
        cameras[i]->lookAt(ofVec3f(0.0f,  0.0f,  0.0f));
        cameras[i]->dolly(-0.5f);
        cameras[i]->setNearClip(.2f);
        cameras[i]->setFarClip(1.1f);
        
        triGrabPoints[i] = new ofVec3f[3];
        for (int j=0;j<3;j++) {
            ofVec3f tmp =cameras[i]->worldToScreen(icoVerts[icoUniqSides[i*3+j]], viewGrid[i]);
            tmp.x += 0.8;
            triGrabPoints[i][j] =tmp;
        };
    }
    cam.setupPerspective(false);
    
    cam.setNearClip(.5f);
    cam.setFarClip(10.1f);
//    cam.setPosition(0.755f, -2.64f, -0.5f);
    cam.setPosition(0, -2.64f, -2.5f);
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
//	float xOffset = ofGetWidth() / 2;
//	float yOffset = ofGetHeight() / (N_CAMERAS / 2);
    float grid_w = 100;
    sideFbo.allocate(grid_w, grid_w, GL_RGBA);

    float xOffset = ofGetWidth()-grid_w*2;
	float yOffset = 100;
    

	viewMain.x = grid_w*2;
	viewMain.y = 0;
	viewMain.width = xOffset;
	viewMain.height = ofGetHeight();
    

	for(int i = 0; i < N_CAMERAS; i++){
        if (i < N_CAMERAS/2) {
            viewGrid[i].x = 0;
            viewGrid[i].y = grid_w * i;
            viewGrid[i].width = grid_w;
            viewGrid[i].height = grid_w;
        } else {
            viewGrid[i].x = grid_w;
            viewGrid[i].y = grid_w * (i-N_CAMERAS/2);
            viewGrid[i].width = grid_w;
            viewGrid[i].height = grid_w;
        }
	}
}

//--------------------------------------------------------------
void testApp::update(){
    cam.setDistance(x_pos);
    
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		// load grayscale depth image from the kinect source
//		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        int w = 640;
        int h = 480;
        
        int step = 1, cnt = 0;
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w; x += step, cnt++) {
                if(kinect.getDistanceAt(x, y) > 0) {
//                    kinectMesh.addColor(ofColor::fromHsb(kinect.getDistanceAt(x, y), 255, 255));
                    kinectMesh.setVertex(cnt, kinect.getWorldCoordinateAt(x, y));
                }
            }
        }
    }
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
//
    if (drawKinect == true) {
        drawPointCloud();
        icoMesh.draw(OF_MESH_WIREFRAME);
    } else {
        doShader ? drawInIco() : drawScene(true);  
    }
//    ofPixels * depth = kinect.getDepthPixelsRef();
//    screenImg.setFromPixels(kinect.getDepthPixelsRef(), kinect.getHeight(), kinect.getWidth(), OF_IMAGE_GRAYSCALE);
//    screenImg.draw(200, 200);

//    drawScene(false);

//    if (doShader) icoMesh.draw(OF_MESH_WIREFRAME);

    cam.end();
//    kinectMesh.drawVertices();
    ofSetColor(0, 255, 255);
    ofDrawArrow(cam.worldToScreen(icoVerts[0],viewMain), cam.worldToScreen(icoVerts[1],viewMain));
    screenImg.grabScreen(viewMain.x, viewMain.y, viewMain.width, viewMain.height);

    if (drawKinect == true) getSides(); // draw from diff sides and et colors
    
    // --- SEND IMG TO UDP ---
    drawToUdp(screenImg);
    
    ofDisableDepthTest();
    ofSetColor(255, 255, 255);
    
    for (int i=0; i<12; i++){
		// we do a manual scale by scaling the mesh vertex coord by 50.f to render the point at the vertex position
		ofDrawBitmapString(ofToString(i), cam.worldToScreen(icoVerts[i], viewMain));
        
	}
    ofEnableDepthTest();
    
//	//
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

    gui.draw();
//    glDepthFunc(GL_LESS);
}

void testApp::getSides() {
    for(int i = 0; i < N_CAMERAS; i++){
		ofSetColor(255, 255, 255);
        ofNoFill();
        ofRect(viewGrid[i]);
        //        sideFbo.begin();
        cameras[i]->begin(viewGrid[i]);
        
		!doShader ? drawScene(false) : drawInIco();
        //        tan(60) = (triPoints[1].y - triPoints[0].y) / (xtriPoints[1].x - triPoints[0].x);
        ofFill();
        ofSetColor(200, 200, 200);
        
        for (int j=0; j<12; j++){
            ofDrawBitmapString(ofToString(j), icoVerts[j]);
        }
        ofFill();
		cameras[i]->end();
        
        ofSetColor(255);
        if (doShader) {
            ofDrawArrow(triGrabPoints[i][1], triGrabPoints[i][0]);
            ofDrawArrow(triGrabPoints[i][1], triGrabPoints[i][2]);
            ofDrawArrow(triGrabPoints[i][0], triGrabPoints[i][2]);
        }
        ofDrawBitmapString(ofToString(i+0), triGrabPoints[i][0]);
        ofDrawBitmapString(ofToString(i+1), triGrabPoints[i][1]);
        ofDrawBitmapString(ofToString(i+2), triGrabPoints[i][2]);
        //        float dist = sqrt(pow(triGrabPoints[i][0].x-triGrabPoints[i][2].x, 2)+pow(triGrabPoints[i][0].y-triGrabPoints[i][2].y, 2));
        //        ofDrawBitmapString(ofToString(dist), triGrabPoints[i][0]+triGrabPoints[i][2]);
        //        ofDrawSphere(triPoints[0], 1.f);
        //        sideFbo.end();
        //        sideFbo.
        //        sideFbo.draw(viewGrid[i].x, viewGrid[i].y, viewGrid[i].width, viewGrid[i].height);
	}

}

void testApp::drawScene(bool is_main) {

//    ofSetBackgroundColor(10, 10, 10);
    glEnable(GL_DEPTH_TEST);
    ofSetColor(0, 200, 255);
    glLineWidth(.5f);
    
    if (!doShader)icoMesh.draw(OF_MESH_WIREFRAME);

    ofFill();
//    ofSetColor(0, 255.0f * sin(ofGetFrameNum()*.8f), 100, 250);
    ofSetColor(HsvToRgb(300*sin(ofGetFrameNum()*.03f), 255, 255));
//    ofDrawSphere( 1.5f*sin(ofGetFrameNum()*.07f));
//    ofDrawSphere(1.0f*sin(ofGetFrameNum()*coeff), 1.f*cos(ofGetFrameNum()*coeff), .0f,  .6f);//sin(ofGetFrameNum()*.02f)

//    boxMesh.box( 1.2f*sin(ofGetFrameNum()*.1f) );
//    boxMesh = boxMesh.xbox(1.5f*cos(ofGetFrameNum()*.06f), 1.5f, 1.5f);
//    ofDrawBox(1.2f*sin(ofGetFrameNum()*.1f));
    
//    ofSetColor(200.0f*sin(ofGetFrameNum()* coeff * .1f), 200, 0, 200);
//    ofDrawCone(0, 0, 0, 1.0f*sin(ofGetFrameNum()*.05f), 1.5f*sin(ofGetFrameNum()*.07f));
    ofSetColor(color);
    pos.set(1.5*ofVec3f(sin(ofGetFrameNum()*coeff), 0, 0));
    ofDrawBox(pos, .2f, 3.f, 3.f);
//    ofPushMatrix();
//        //    ofTranslate(0, 0, 0);
//        ofTranslate(pos);
//        boxMesh.draw();
//    ofPopMatrix;
//    ofSetColor(150, 0, 200.0f*sin(ofGetFrameNum()*.01f), 200);
    ofSetColor(color);
//    ofDrawSphere(0.0f, .0f, 0.0f, 1.f*cos(ofGetFrameNum()*0.05f));

}

void testApp::drawPointCloud() {

//	ofMesh mesh;
//	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int yp = 0; yp < KINECT_H; yp += step) {
		for(int x = 0; x < KINECT_W; x += step) {
			if(kinect.getDistanceAt(x, yp) > 0) {
//				kinectMesh.setColor(yp*x, HsvToRgb(ofMap(kinect.getDistanceAt(x, yp), 0, 3000, 0, 360), 255, 255));
//				kinectMesh.setVertex(yp*x, ofVec3f(x, yp, kinect.getDistanceAt(x, yp)));
			}
		}
	}
//    printf("%f x %f x %f ",300, 240, kinect.getDistanceAt(300, 240));
    
	glPointSize(2);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -100); // center the points a bit
	ofEnableDepthTest();
	kinectMesh.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
}

void testApp::drawToUdp(ofImage img) {
    if (udpAvailable) {
        unsigned char * pix = img.getPixels();
        char to_leds [N_LEDS*3];
        for (int i = 0; i<N_LEDS; i++ ) {

            ofColor cl = img.getColor(viewMain.width/2, ofGetHeight()/2-N_LEDS/2+i);
            to_leds[i*3+0] = (char)cl.r;
            to_leds[i*3+1] = (char)cl.b;
            to_leds[i*3+2] = (char)cl.g;
    //        printf("%02X %02X %02X \n", sto_leds[i*3+0], to_leds[i*3+1], to_leds[i*3+2]);
        }
    //    printf("\n \n");
        udpConnection.Send(to_leds, N_LEDS*3);
    ofLine(ofGetWidth()/2+viewMain.width/2, ofGetHeight()/2-N_LEDS/2, ofGetWidth()/2+viewMain.width/2, ofGetHeight()/2+N_LEDS/2);
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

void testApp::drawInIco() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
    
    // draw stencil pattern
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
    icoMesh.draw();
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);
    // draw where stencil's value is 0
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    /* (nothing to draw) */
    // draw only where stencil's value is 1
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    
    drawScene(false);
    
    glDisable(GL_STENCIL_TEST);
    
}


void testApp::exit ()
{
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}
