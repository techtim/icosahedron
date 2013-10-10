#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    setupFinished =false;
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0, 0, 0);

	ofEnableAlphaBlending();    
//	ofSetVerticalSync(false);
    ofSetVerticalSync(true);
	// this uses depth information for occlusion rather than always drawing things on top of each other

    ofSetCircleResolution(64);
    
//  Setup Icosahedron Mesh FIRST, cause every thing depends from it
    icoMesh = icoUtils::get_vertices();

    setupViewports();
    setupCam(0);

    setupScenes();
    
    x_pos = 80;
	cam.getMouseInputEnabled();
//	shader.setGeometryInputType(GL_TRIANGLES);
//	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
//	shader.setGeometryOutputCount(4);
//	shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
    //
    //	printf("Maximum number of output vertices support is: %i\n", shader.getGeometryMaxOutputCount());
    
    // -- GUI --

    ofxGuiSetDefaultWidth(300);
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
	gui.add(filled.set("bFill", true));
	gui.add(radius.set( "radius", 1, 0, 20 ));
	gui.add(center.set("center",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
	gui.add(color.set("color",ofColor(0,200,140,200),ofColor(0,0,0,0),ofColor(255,255,255,255)));
	gui.add(circleResolution.set("circleRes", 5, 3, 90));
	gui.add(drawSides.set("draw Sides",true));
	gui.add(drawKinect.set("draw Kinect", false));
    gui.add(sendUdp.set("UDP", false));
	gui.add(screenSize.set("screenSize", ""));
    gui.add(coeff.set("coef", 0.01, 0, 1));
    gui.add(pos.set("pos", ofVec3f(0,0,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    gui.add(scene_num.set("scene_num", 0, 0, SCENES_NUM));
    gui.add(sceneSel1.set("Sel scene 1", 0, 0, SCENES_NUM));
    gui.add(sceneSel2.set("Sel scene 2", 1, 0, SCENES_NUM));
    
    gui.setPosition(200, 20);
//    gui.setDefaultWidth(300);
    

    font.loadFont("type/verdana.ttf", 100, true, false, true, 0.4, 72);
	doShader = false;
    
    x_pos=0, y_pos=60;

//    boxMesh.addVertices(&boxVertices[0], 8);
//	boxMesh.addNormals(boxNormals,8);
//	boxMesh.addIndices(boxIndices, 3*2*6);
//    boxMesh = boxMesh.box(1.0f, 0.8f, 0.8f);
//    boxMesh.box(0.2f, 1.5f, 1.5f);
    boxMesh = ofMesh::box(0.2f, 1.5f, 1.5f);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    kinectMesh.setMode(OF_PRIMITIVE_POINTS);
    
//    kinectMesh.addVertices(kinectVerts, KINECT_W*KINECT_H);
//	grayImage.allocate(kinect.width, kinect.height);
    // --- INTPUT ---
    oscKinect.setup(KINECT_OSC_PORT);
    
    // --- OUTPUT ---
    udpConnection.Create();
	udpAvailable = udpConnection.Connect(RPI_HOST2, RPI_PORT);
	udpConnection.SetNonBlocking(true);

    sidesImg.allocate(80, 30*2, OF_IMAGE_COLOR);

    indexGrabPixels = new unsigned * [N_CAMERAS];
    for (int i=0; i<N_CAMERAS; i++) {
        indexGrabPixels[i] = new unsigned int [N_CAMERAS];
    }

//    udpAvailable = true;
    setupFinished =true;
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
            icoMesh.getVertices()[icoUniqSides[i*3+0]]+icoMesh.getVertices()[icoUniqSides[i*3+1]]+icoMesh.getVertices()[icoUniqSides[i*3+2]]
        );
//        cameras[i]->setPosition(icoVerts[Faces[i*3+0]]+icoVerts[Faces[i*3+1]]+icoVerts[Faces[i*3+2]]);
        cameras[i]->lookAt(ofVec3f(0.0f,  0.0f,  0.0f));
        cameras[i]->dolly(-0.5f);
        cameras[i]->setNearClip(.2f);
        cameras[i]->setFarClip(1.11f);
        
        triGrabPoints[i] = new ofVec3f[3];
        for (int j=0;j<3;j++) {
            ofVec3f tmp =cameras[i]->worldToScreen(icoMesh.getVertices()[icoUniqSides[i*3+j]], viewGrid[i]);
//            tmp.x += 0.8;
            triGrabPoints[i][j] =tmp;
        };
    }
    cam.setupPerspective(false);
    
    cam.setNearClip(.5f);
    cam.setFarClip(7.1f);
//    cam.setPosition(0.755f, -2.64f, -0.5f);
    cam.setPosition(0.f, 0.f, -5.5f);
    
    selectCam = 0;
//    cam.setPosition(icoMesh.getVertices()[Faces[selectCam*3+0]]+icoMesh.getVertices()[Faces[selectCam*3+1]]+icoMesh.getVertices()[Faces[selectCam*3+2]]);
    // 1.907f, 1.387f, 3.087f
    cam.lookAt(ofVec3f(0.0f,  0.0f,  0.0f));
//    cam.setDistance(3.89f);
    cam.setDistance(2.f);
    x_pos = 80;
	cam.getMouseInputEnabled();
}

void testApp::setupViewports(){
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

void testApp::setupScenes() {
//    icoScene * sa = ;
    scenesVec.push_back(new icoScene0());
    icoScene * sa = new icoScene1();
    scenesVec.push_back(sa);
    icoScene * se = new icoScene2();
    scenesVec.push_back(se);
    icoScene * sf = new icoScene3();
    scenesVec.push_back(sf);
    icoScene * ss = new icoScene4();
    scenesVec.push_back(ss);
    icoScene * sw = new icoScene5();
    scenesVec.push_back(sw);
}

//--------------------------------------------------------------
void testApp::update(){
    if (oscKinect.hasWaitingMessages()){
        
        kinectMesh.clear();
        ofMatrix4x4 S = ofMatrix4x4::newScaleMatrix(-100, -100, -50);
        ofMatrix4x4 T = ofMatrix4x4::newTranslationMatrix(ofVec3f(0,0,0));
        while(oscKinect.hasWaitingMessages()){
            ofxOscMessage m;
            oscKinect.getNextMessage(&m);

            if(m.getAddress() == "/kinect/point"){
                ofVec3f tmp = ofVec3f(KINECT_W/2-
                                      m.getArgAsInt32(1), 127.f - m.getArgAsInt32(3), KINECT_H/2-m.getArgAsInt32(2));
                tmp.normalize();
    //            printf("%f %f %f \n", tmp.x, tmp.y, tmp.z );
                kinectMesh.addVertex(tmp);
                ofVec3f norm = ofVec3f(m.getArgAsInt32(4), m.getArgAsInt32(5), 0);
                norm.normalize();
                kinectMesh.addNormal(norm);
                kinectMesh.addColor(ofFloatColor(255.f));

            } else if (m.getAddress().compare("audio")){
                
//                for (int i=0; i< m.getNumArgs(); i++) printf("%f \n", m.getArgAsFloat(i)); printf("----------------\n\n");
                if (m.getAddress() == "/audio/lowTrig"){
                    lowTrig = m.getArgAsInt32(0);
                } else if (m.getAddress() == "/audio/lowFol"){
                    lowFol = m.getArgAsFloat(0)*2.f;
                } else if (m.getAddress() == "/audio/midTrig"){
                    midTrig = m.getArgAsInt32(0);
                } else if (m.getAddress() == "/audio/midFol"){
                    midFol = m.getArgAsFloat(0)*2.f;
                } else if (m.getAddress() == "/audio/hiTrig"){
                    hiTrig = m.getArgAsInt32(0);
                } else if (m.getAddress() == "/audio/hiFol"){
                    hiFol = m.getArgAsFloat(0)*2.f;
                }
                
                scenesVec[sceneSel1%scenesVec.size()]->updateAudio(lowFol, lowTrig, midFol, midTrig, hiFol, hiTrig);
                scenesVec[sceneSel2%scenesVec.size()]->updateAudio(lowFol, lowTrig, midFol, midTrig, hiFol, hiTrig);
                lowTrig = 0, midTrig = 0, hiTrig = 0;
            }
            
        }

        color.set(ofColor(color.get().r, color.get().g, color.get().b, lowFol*255.f));
    }
}
//--------------------------------------------------------------
void testApp::draw() {

    ofEnableDepthTest();
//    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    cam.begin(viewMain);
//
    if (drawKinect == true) {
//        drawPointCloud();

        kinectMesh.draw();
        icoMesh.draw(OF_MESH_WIREFRAME);
    } else {
        doShader ? drawInIco() : drawScene(true);  
    }

//    drawScene(false);

//    if (doShader) icoMesh.draw(OF_MESH_WIREFRAME);
    ofDrawAxis(100);
    cam.end();
//    kinectMesh.drawVertices();
//    ofSetColor(0, 255, 255);
//    ofDrawArrow(cam.worldToScreen(icoMesh.getVertices()[0],viewMain), cam.worldToScreen(icoMesh.getVertices()[1],viewMain));
    screenImg.grabScreen(viewMain.x, viewMain.y, viewMain.width, viewMain.height);

    if (drawSides == true) getSides(); // draw from diff sides and et colors
    
    
//    udpImageMat.
    ofDisableDepthTest();
    ofSetColor(255, 255, 255);
    
    for (int i=0; i<12; i++){
		// we do a manual scale by scaling the mesh vertex coord by 50.f to render the point at the vertex position
		ofDrawBitmapString(ofToString(i), cam.worldToScreen(icoMesh.getVertices()[i], viewMain));
        
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
    ofVec3f tmp = icoMesh.getVertices()[Faces[selectCam*3+0]]+icoMesh.getVertices()[Faces[selectCam*3+1]]+icoMesh.getVertices()[Faces[selectCam*3+2]];
    ofDrawBitmapString("O_POS="+ofToString(tmp),150.5f, 50.0f);
    
    ofDrawBitmapString(ofToString(cam.getLocalTransformMatrix()), 521.5f, 60.0f);

    gui.draw();
//    glDepthFunc(GL_LESS);
}

void testApp::getSides() {
//    if (!setupFinished) return;
//    glEnable(GL_CULL_FACE);
//    glCullFace (GL_FRONT);
    int i;
    for(i= 0; i < N_CAMERAS; i++){
		ofSetColor(255, 255, 255);
//        ofNoFill();
//        ofRect(viewGrid[i]);

        cameras[i]->begin(viewGrid[i]);
        
		!doShader ? drawScene(false) : drawInIco();

        ofFill();
        ofSetColor(200, 200, 200);
        
//        for (int j=0; j<12; j++){
//            ofDrawBitmapString(ofToString(j), icoMesh.getVertices()[j]);
//        }
        ofFill();
		cameras[i]->end();

        ofSetColor(255);
//        ofDrawBitmapString(ofToString(icoUniqSides[i*3+0]), triGrabPoints[i][0]);
//        ofDrawBitmapString(ofToString(icoUniqSides[i*3+1]), triGrabPoints[i][1]);
//        ofDrawBitmapString(ofToString(icoUniqSides[i*3+2]), triGrabPoints[i][2]);
	}
    
//    OF_IMAGE_COLORALPHA
    sidesGrabImg.grabScreen(viewGrid[0].x, viewGrid[0].y, viewGrid[N_CAMERAS-1].x+viewGrid[0].width, viewGrid[N_CAMERAS-1].y+viewGrid[N_CAMERAS-1].height);
//    sidesGrabImg.setImageType(OF_IMAGE_COLOR);
//    sidesImageMat = cv::Mat(sidesGrabImg.height, sidesGrabImg.width, CV_8UC3, sidesGrabImg.getPixels(), 0);
//    udpImageMat = cv::Mat(36, 80, CV_8UC3);


    sidesImg.allocate(80, 30, OF_IMAGE_COLOR_ALPHA);
//    sidesImg.setImageType(OF_IMAGE_COLOR);

    int row_cntr = 0;
    unsigned char *output = new unsigned char [30*80*3]  ; // (unsigned char*)(sidesImageMat.data);
        
    unsigned int cntr = 0;
    for (int cam=0; cam<N_CAMERAS; cam++) {
        ofVec2f camVerts [] = {
            ofVec2f(icoUniqSides[cam*3+0], icoUniqSides[cam*3+1]),
            ofVec2f(icoUniqSides[cam*3+1], icoUniqSides[cam*3+2]),
            ofVec2f(icoUniqSides[cam*3+2], icoUniqSides[cam*3+0]),
        };
ofSetColor(0, 255, 255);
        for (int v_num=0; v_num<3; v_num++ ) {

            for (int j=0;j<30;j++) {
                        
                if (camVerts[v_num].x == icoGrabSides[j].x && camVerts[v_num].y == icoGrabSides[j].y) {
                    
    //                    ofSetColor(rand()%255, rand()%255, rand()%255);
                    ofVec2f vert1 = cameras[cam]->worldToScreen(icoMesh.getVertex(icoGrabSides[j].x), viewGrid[cam]);
                    ofVec2f vert2 = cameras[cam]->worldToScreen(icoMesh.getVertex(icoGrabSides[j].y), viewGrid[cam]);
                    ofSetColor(0, 255, 255);
                    
                    ofDrawBitmapString(ofToString(j), vert1);
//                    ofDrawBitmapString(ofToString(j), vert2);
//                    ofDrawSphere(vert1, 3);
//                    ofDrawSphere(vert2, 3);
                    for (int pix_num=0;pix_num<LEDS_NUM_IN_SIDE;pix_num++) {
                        ofVec2f tmp = vert1.getInterpolated(vert2, (pix_num+2)*0.0120f);
                        
//                        ofDrawSphere(tmp, 1.);
//                        udpImageMat.at<ofVec3f>(j, pix_num-1) = sidesImageMat.at<ofVec3f>((int)tmp.y, (int)tmp.x);
                        ofColor col = sidesGrabImg.getColor((int)tmp.x, (int)tmp.y);
                        output[j*LEDS_NUM_IN_SIDE*3+pix_num*3+0] = col.r;
                        output[j*LEDS_NUM_IN_SIDE*3+pix_num*3+1] = col.b;
                        output[j*LEDS_NUM_IN_SIDE*3+pix_num*3+2] = col.g;
                    }
                    cntr++;
                    break;
                } else if (camVerts[v_num].x == icoGrabSides[j].y && camVerts[v_num].y == icoGrabSides[j].x) {
                    
                    ofVec2f vert1 = cameras[cam]->worldToScreen(icoMesh.getVertex(icoGrabSides[j].x), viewGrid[cam]);
                    ofVec2f vert2 = cameras[cam]->worldToScreen(icoMesh.getVertex(icoGrabSides[j].y), viewGrid[cam]);
                    
                    ofDrawBitmapString(ofToString(j), vert1);
//                    ofDrawBitmapString(ofToString(j), vert2);
//                    ofSetColor(255, 0, 0);
//                    ofDrawSphere(vert1, 3);
//                    ofDrawSphere(vert2, 3);
                    for (int pix_num=0;pix_num<LEDS_NUM_IN_SIDE;pix_num++) {
                        ofVec2f tmp = vert1.getInterpolated(vert2, (pix_num+2)*0.0120f);
                        
                        //                        ofDrawSphere(tmp, 1.);
//                        udpImageMat.at<ofVec3f>(j, pix_num-1) = sidesImageMat.at<ofVec3f>((int)tmp.y, (int)tmp.x);
                        ofColor col = sidesGrabImg.getColor(tmp.x, tmp.y);

                        output[j*LEDS_NUM_IN_SIDE*3+pix_num*3+0] = col.r;
                        output[j*LEDS_NUM_IN_SIDE*3+pix_num*3+1] = col.b;
                        output[j*LEDS_NUM_IN_SIDE*3+pix_num*3+2] = col.g;
                    }
//                    printf("MAT J: %i  CAM: %i  CNTR: %i\n", j, cam, cntr);
                    cntr++;
                    break;

                }
                
    //                ofVec3f tmp =cameras[i]->worldToScreen(icoMesh.getVertex(j)), viewGrid[cam]);
    //                //            tmp.x += 0.8;
    //                triGrabPoints[i][j] = tmp;
    //                ofDrawSphere(tmp, 2);
            };
        };
        triGrabPoints[cam][0],
        triGrabPoints[cam][1];
        for (int j=0; j<30; j++) {};
            
    }
//    printf("CNTR: %i\n", cntr);
//    printf("MAT COLS: %i  ROWS: %i  CNTR: %i\n", udpImageMat.cols, udpImageMat.rows, cntr);
//    icoUtils::drawMat(udpImageMat, 20, 620, udpImageMat.cols, udpImageMat.rows);
    // --- SEND IMG TO UDP ---
    ofSetColor(255);
    sidesImg.setFromPixels(output, 80, 30, OF_IMAGE_COLOR);
    sidesImg.draw(20, 620);
    drawToUdp(output);
    
    delete [] output;
//    udpImageMat;
//    icoUtils::drawMat(sidesImageMat, 0, 0, sidesImageMat.cols, sidesImageMat.rows);
//    glDisable(GL_CULL_FACE);
}

void testApp::drawScene(bool is_main) {

//    ofSetBackgroundColor(10, 10, 10);
    glEnable(GL_DEPTH_TEST);
    ofSetColor(0, 200, 255);
    glLineWidth(.5f);
    
//    icoMesh.draw(OF_MESH_WIREFRAME);
    
    if (kinectMesh.getNumVertices()>0){
        ofColor col = ofColor(255);
        col.setHsb(255*sin(ofGetFrameNum()*coeff*.1f), 255, 255);
        ofSetColor(col);
        ofFill();
        for (int i=0;i<kinectMesh.getNumVertices(); i++) {
            ofDrawSphere(kinectMesh.getVertices()[i].x,kinectMesh.getVertices()[i].y,kinectMesh.getVertices()[i].z, .3f);
        }
    }
    ofFill();

    // --------- SCENES ---------
    ofSetColor(HsvToRgb(1+243*fabs(sin(ofGetFrameNum()*.03f)), 255, 255));
    scenesVec[sceneSel1%scenesVec.size()]->draw();

    ofSetColor(color);
    scenesVec[sceneSel2%scenesVec.size()]->draw();

    ofSetColor(255);
    
    
//    sceneManager[1].draw();
//    ofSetColor(0, 255.0f * sin(ofGetFrameNum()*.8f), 100, 250);
    
//    ofDrawSphere( 1.5f*sin(ofGetFrameNum()*.07f));
//    ofDrawSphere(1.0f*sin(ofGetFrameNum()*coeff), 1.f*cos(ofGetFrameNum()*coeff), .0f,  .6f);//sin(ofGetFrameNum()*.02f)

//    boxMesh.box( 1.2f*sin(ofGetFrameNum()*.1f) );
//    boxMesh = boxMesh.xbox(1.5f*cos(ofGetFrameNum()*.06f), 1.5f, 1.5f);
//    ofDrawBox(1.2f*sin(ofGetFrameNum()*.1f));
    
    ofSetColor(200.0f*unsigned(sin(ofGetFrameNum()*coeff)),200, 0, 200);
//    ofDrawCone(0, 0, 0, 1.0f*sin(ofGetFrameNum()*.05f), 1.5f*sin(ofGetFrameNum()*.07f));
    
    
    

    
    ofSetColor(HsvToRgb(unsigned(255*sin(ofGetFrameNum()*.03f)), 255, 255));
//    pos.set(1.5*ofVec3f(sin(ofGetFrameNum()*coeff), 0, 0));
//    ofDrawBox(pos, .2f, 3.f, 3.f);
//    ofPushMatrix();
//        //    ofTranslate(0, 0, 0);
//        ofTranslate(pos);
//        boxMesh.draw();
//    ofPopMatrix;
//    ofSetColor(150, 0, 200.0f*sin(ofGetFrameNum()*.01f), 200);

    ofDrawSphere(0.0f, .0f, 0.0f, radius*0.1);

}

void testApp::drawPointCloud() {

//	ofMesh mesh;
//	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
    
	glPointSize(2);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
        ofScale(1, -1, -1);
        ofTranslate(0, 0, -100); // center the points a bit
        ofEnableDepthTest();
        kinectMesh.drawVertices();
//        ofDisableDepthTest();
	ofPopMatrix();
}

void testApp::drawToUdp(ofImage img) {
    if (sendUdp) {
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

void testApp::drawToUdp(unsigned char * pix) {
    if (sendUdp) {
//        img.at<ofColor>();
        char to_leds [N_LEDS*3];
        for (int i = 0; i<N_LEDS*3; i++ ) {
            to_leds[i] = (char)pix[i];
            //        printf("%02X %02X %02X \n", sto_leds[i*3+0], to_leds[i*3+1], to_leds[i*3+2]);
        }
        //    printf("\n \n");
        udpConnection.Send(to_leds, N_LEDS*3);
//        ofLine(ofGetWidth()/2+viewMain.width/2, ofGetHeight()/2-N_LEDS/2, ofGetWidth()/2+viewMain.width/2, ofGetHeight()/2+N_LEDS/2);
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

void testApp::onMessageReceived(ofxOscMessage &msg) {
    
}

void testApp::exit ()
{
    for (int i=0; i<N_CAMERAS;i++) delete [] triGrabPoints[i];
    delete [] triGrabPoints;
}
