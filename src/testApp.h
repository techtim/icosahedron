#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "icoUtils.h"
#include "ofxNetwork.h"
#include "ofxMesh.h"
#include "OrthoCamera.h"

//#include "ofxOpenCv.h"
#include "ofxKinect.h"

#define N_CAMERAS 12
//#define N_LEDS 800
#define N_LEDS 640
#define RPI_HOST1 "192.168.2.222"
#define RPI_HOST2 "192.168.2.223"
#define RPI_PORT 2525

#define KINECT_W 640
#define KINECT_H 480

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();

        void draw();
        void drawScene(bool is_main);
        void drawFirstInSecond ();
        void drawInIco();
        void drawToUdp(ofImage img);

        void drawPointCloud();

        void getSides();
    
        void exit();
    
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

        /// --- CAMERAS SETUP ---
        ofEasyCam cam;
//        orthoCamera * cameras[N_CAMERAS];
//        ofEasyCam * cameras[N_CAMERAS];
        ofCamera * cameras[N_CAMERAS];
        ofFbo sideFbo;
        unsigned int selectCam;
        //viewports
        ofRectangle viewMain;
        ofRectangle viewGrid[N_CAMERAS];

        // --- KINECT ---
        ofxKinect kinect;
//        ofxGrayscaleImage grayImage;
//        ofxCvContourFinder contourFinder;

        ofVboMesh kinectMesh;
        ofVec3f kinectVerts[KINECT_W*KINECT_H];
        bool bThreshWithOpenCV;
        bool bDrawPointCloud;
        
        int nearThreshold;
        int farThreshold;

    
        ofVbo vbo;
        
        ofVboMesh boxMesh, icoMesh, resultCsg;
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
        ofxButton drawSides;
        ofxButton drawKinect;
        ofParameter<int> shutter;
        ofParameter<bool> lamp;
        ofParameter<ofVec3f> pos;
        ofParameter<float> coeff;
        ofParameter<string> screenSize;
    
        // --- OUTPUT ---
        ofxUDPManager udpConnection;
        ofImage screenImg;
        bool udpAvailable;
        ofVec3f ** triGrabPoints;
    
    
    
    void setupCamAlter(int main_cam_num){
        
        for (int i=0; i<N_CAMERAS; i++) {
//            cameras[i] = new ofEasyCam();
//            cameras[i]->setupPerspective(true);
            
            //        cameras[i]->setNearClip(2.9f);
            //        cameras[i]->setFarClip(3.1f);
            cameras[i]->setNearClip(.75f);
            cameras[i]->setFarClip(1.f);
            
            //    cameras[i](0.0f, 0.0f, 5.0f);
            cameras[i]->setPosition(icoVerts[Faces[i*3+0]]+icoVerts[Faces[i*3+1]]+icoVerts[Faces[i*3+2]]);
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
    
    ofColor HsvToRgb (float hue, float satur, float value )
    {
        float r=0, g=0, b=0, fr, q, t, p;
        int i, H;
        ofColor rgbColor;
        rgbColor.set(r, g, b, 0);
        H = (int)(hue / 60);
        fr = hue/60 - H;
        p = (value * (255 - satur))/255;
        q = (value * (255 - satur * fr))/255;
        t = (value * (255 - satur * (1.0 - fr)))/255;
        switch (H) {
            case 0:
                rgbColor.set(value, t, p, value);
                break;
            case 1:
                rgbColor.set(q, value, p, value);
                break;
            case 2:
                rgbColor.set(p, value, t, value);
                break;
            case 3:
                rgbColor.set(p, q, value, value);
                break;
            case 4:
                rgbColor.set(t, p, value, value);
                break;
            case 5:
                rgbColor.set(value, p, q, value);
                break;
            default:
                rgbColor.set(255, 255, 255, 255);
        }
        return rgbColor;
    }

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