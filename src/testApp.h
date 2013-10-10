#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "icoUtils.h"
#include "ofxNetwork.h"
#include "ofxMesh.h"
#include "OrthoCamera.h"
#include "ofxOsc.h"
#include "icoScene.h"

#include "ofxOpenCv.h"

#define N_CAMERAS 12
//#define N_LEDS 800
#define N_LEDS 400
#define SCENES_NUM 10

#define RPI_HOST1 "192.168.2.50"
#define RPI_HOST2 "192.168.2.51" //223"
#define RPI_PORT 2525

#define LEDS_NUM_IN_SIDE 80

#define KINECT_OSC_HOST "localhost"
#define KINECT_OSC_PORT 1337
//#define AUDIO_OSC_PORT 1337

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
        void drawToUdp(unsigned char * img);

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
    
        void onMessageReceived(ofxOscMessage &msg);
    
        void glDonut(int x, int y, int rad, int inrad, float r, float g, float b, float a);
        void setupCam(int main_cam_num);
        void setupViewports();
        
        bool setupFinished;
    
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

        ofVboMesh kinectMesh;
        ofVec3f kinectVerts[KINECT_W*KINECT_H];
        bool bThreshWithOpenCV;
        bool bDrawPointCloud;
        
        int nearThreshold;
        int farThreshold;
    
        ofVbo vbo;
        
        ofVboMesh boxMesh, icoMesh;
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
        ofParameter<bool> drawSides;
        ofParameter<bool> drawKinect;
        ofParameter<bool> sendUdp;
        ofParameter<int> shutter;
        ofParameter<bool> lamp;
        ofParameter<ofVec3f> pos;
        ofParameter<float> coeff;
        ofParameter<int> scene_num;
        ofParameter<int> sceneSel1;
        ofParameter<int> sceneSel2;
        ofParameter<string> screenSize;
    
        // --- INPUT ----
        ofxOscReceiver oscKinect;

        ofVec3f * kinectPos;
        ofVec3f * kinectVel;
        unsigned int * kinectLabels;
    vector<ofVec3f> kinectPoints;
    
        // --- Audio ---
        float  lowFol, midFol, hiFol;
        int lowTrig, midTrig, hiTrig;
    
        // --- OUTPUT ---
        ofxUDPManager udpConnection;
        ofImage screenImg;
        ofImage sidesImg;
        ofImage sidesGrabImg;
        bool udpAvailable;
        ofVec3f ** triGrabPoints;
        ofVec3f * sidesGrabPoints;
        unsigned int ** indexGrabPixels;
//        cv::Mat sidesImageMat, udpImageMat;

        // --- SCENES ---
        void setupScenes();
//        icoScene * sceneManager[SCENES_NUM];
        vector <icoScene *> scenesVec;
        icoScene1 iScene1;
        icoScene2 iScene2;
        
    
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
