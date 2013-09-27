#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "icoUtils.h"
#include "ofxCarveCSG.h"
#include "ofxNetwork.h"
#include "ofxMesh.h"
#include "OrthoCamera.h"

#define N_CAMERAS 12
#define N_LEDS 800
#define RPI_HOST "192.168.2.222"
#define RPI_PORT 2525

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();

        void draw();
        void drawScene(bool is_main);
        void drawFirstInSecond ();
        void drawInIco();
        void drawToUdp(ofImage img);

        void fixDepth ();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        void glDonut(int x, int y, int rad, int inrad, float r, float g, float b, float a);
        void setupCam(int main_cam_num);
        void setupViewports();
                
        ofShader shader;
        bool doShader;
        vector<ofPoint> points;
        
        ofVec3f v[12];
        ofVec3f n[20];
        ofFloatColor c[20];
        ofFloatColor cWhite[20];

        ofEasyCam cam;
        orthoCamera * cameras[N_CAMERAS];
//        ofEasyCam * cameras[N_CAMERAS];
    
        ofFbo sideFbo;
    
        unsigned int selectCam;
        //viewports
        ofRectangle viewMain;
        ofRectangle viewGrid[N_CAMERAS];

        ofVbo vbo;
        
        ofMesh boxMesh, icoMesh, resultCsg;
        ofVbo boxVbo;
        ofVec3f icoSpin;
        ofVec3f icoNormals[3];
    
        ofTrueTypeFont font;
        int x_pos, y_pos;
    
    // -- GUI --
        ofxPanel gui;
        
        ofParameter<float> radius;
        ofParameter<ofColor> color;
        ofParameter<ofVec2f> center;
        ofParameter<int> circleResolution;
        ofParameter<bool> filled;
        ofxButton twoCircles;
        ofxButton ringButton;
        ofParameter<string> screenSize;
    
        // --- OUTPUT ---
        ofxUDPManager udpConnection;
        ofImage screenImg;
        bool udpAvailable;
    
    
    
    
    void setupCamAlter(int main_cam_num){
        
        for (int i=0; i<N_CAMERAS; i++) {
//            cameras[i] = new ofEasyCam();
//            cameras[i]->setupPerspective(true);
            
            //        cameras[i]->setNearClip(2.9f);
            //        cameras[i]->setFarClip(3.1f);
            cameras[i]->setNearClip(.75f);
            cameras[i]->setFarClip(1.f);
            
            //    cameras[i](0.0f, 0.0f, 5.0f);
//            cameras[i]->setPosition(icoVerts[Faces[i*3+0]]+icoVerts[Faces[i*3+1]]+icoVerts[Faces[i*3+2]]);
            cameras[i]->lookAt(ofVec3f(0.0f,  0.0f,  0.0f));
//            cameras[i]->setDistance(1.75f);
        }
        cam.setupPerspective(true);
        
        cam.setNearClip(3.0f);
        cam.setFarClip(5.1f);
        cam.setPosition(0.755f, -2.64f, -0.5f);
        selectCam = 0;
        //    cam.setPosition(icoVerts[Faces[selectCam*3+0]]+icoVerts[Faces[selectCam*3+1]]+icoVerts[Faces[selectCam*3+2]]);

        cam.lookAt(ofVec3f(0.0f,  0.0f,  0.0f));

        cam.setDistance(2.f);
        x_pos = 80;
        cam.getMouseInputEnabled();
    }

};

//const ofIndexType Faces[] = {
//    2, 1, 0,
//    3, 2, 0,
//    4, 3, 0,
//    5, 4, 0,
//    1, 5, 0,
//    11, 6,  7,
//    11, 7,  8,
//    11, 8,  9,
//    11, 9,  10,
//    11, 10, 6,
//    1, 2, 6,
//    2, 3, 7,
//    3, 4, 8,
//    4, 5, 9,
//    5, 1, 10,
//    2,  7, 6,
//    3,  8, 7,
//    4,  9, 8,
//    5, 10, 9,
//    1, 6, 10 };
//const float Verts[] = {
//    0.000f,  0.000f,  1.000f,
//    0.894f,  0.000f,  0.447f,
//    0.276f,  0.851f,  0.447f,
//    -0.724f,  0.526f,  0.447f,
//    -0.724f, -0.526f,  0.447f,
//    0.276f, -0.851f,  0.447f,
//    0.724f,  0.526f, -0.447f,
//    -0.276f,  0.851f, -0.447f,
//    -0.894f,  0.000f, -0.447f,
//    -0.276f, -0.851f, -0.447f,
//    0.724f, -0.526f, -0.447f,
//    0.000f,  0.000f, -1.000f };
//
//const ofVec3f icoVerts[] = {
//    ofVec3f(0.000f,  0.000f,  1.000f),
//    ofVec3f(0.894f,  0.000f,  0.447f),
//    ofVec3f(0.276f,  0.851f,  0.447f),
//    ofVec3f(-0.724f,  0.526f,  0.447f),
//    ofVec3f(-0.724f, -0.526f,  0.447f),
//    ofVec3f(0.276f, -0.851f,  0.447f),
//    ofVec3f(0.724f,  0.526f, -0.447f),
//    ofVec3f(-0.276f,  0.851f, -0.447f),
//    ofVec3f(-0.894f,  0.000f, -0.447f),
//    ofVec3f(-0.276f, -0.851f, -0.447f),
//    ofVec3f(0.724f, -0.526f, -0.447f),
//    ofVec3f(0.000f,  0.000f, -1.000f)
//};
//
//const ofVec3f icoIndices[] = {
//    ofVec3f(2, 1, 0),
//    ofVec3f(3, 2, 0),
//    ofVec3f(4, 3, 0),
//    ofVec3f(5, 4, 0),
//    ofVec3f(1, 5, 0),
//    ofVec3f(11, 6,  7),
//    ofVec3f(11, 7,  8),
//    ofVec3f(11, 8,  9),
//    ofVec3f(11, 9,  10),
//    ofVec3f(11, 10, 6),
//    ofVec3f(1, 2, 6),
//    ofVec3f(2, 3, 7),
//    ofVec3f(3, 4, 8),
//    ofVec3f(4, 5, 9),
//    ofVec3f(5, 1, 10),
//    ofVec3f(2,  7, 6),
//    ofVec3f(3,  8, 7),
//    ofVec3f(4,  9, 8),
//    ofVec3f(5, 10, 9),
//    ofVec3f(1, 6, 10)
//};

const ofIndexType icoUniqSides[] = {
    0, 1, 2, //0
    0, 4, 5, //1
    0, 4, 3, //2
    4, 9, 8, //3
    3, 8, 7,
    3, 7, 2,
    2, 6, 1,
    1, 10, 5, //7
    5, 10, 9, //8
    7, 6, 11, //9
    10, 6, 11,
    8, 9, 11, //11
    
};

const ofVec3f boxVertices[] = {
	
    ofVec3f(-0.7f, -0.7f,  0.7f),        // front square vertices
    ofVec3f( 0.7f, -0.7f,  0.7f),
    ofVec3f( 0.7f,  0.7f,  0.7f),
    ofVec3f(-0.7f,  0.7f,  0.7f),
    
    ofVec3f(-0.7f, -0.7f, -0.7f),        // back square vertices
    ofVec3f( 0.7f, -0.7f, -0.7f),
    ofVec3f( 0.7f,  0.7f, -0.7f),
    ofVec3f(-0.7f,  0.7f, -0.7f),
    
};

const ofIndexType boxIndices[] = {
    // -- winding is counter-clockwise (facing camera)
    0,1,2,		// pos z
    0,2,3,
    1,5,6,		// pos x
    1,6,2,
    2,6,7,		// pos y
    2,7,3,
    
    // -- winding is clockwise (facing away from camera)
    3,4,0,		// neg x
    3,7,4,
    4,5,1,		// neg y
    4,1,0,
    5,7,6,		// neg z
    5,4,7,
};

const ofVec3f boxNormals[] = {
    ofVec3f( 0,  0,  1),
    ofVec3f( 1,  0,  0),
    ofVec3f( 0,  1,  0),
    ofVec3f(-1,  0,  0),
    ofVec3f( 0, -1,  0),
    ofVec3f( 0,  0, -1),
    ofVec3f(1,0,0), // can be anything, will not be used
    ofVec3f(1,0,0), //  -- " --
};