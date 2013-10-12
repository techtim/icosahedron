#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxOpenCv.h"

#define PI       3.14159265358979323846

const ofIndexType Faces[] = {
    2, 1, 0, //0
    3, 2, 0,
    4, 3, 0, //2
    5, 4, 0, //1
    1, 5, 0,
    11, 6,  7, //9
    11, 7,  8,
    11, 8,  9, //11
    11, 9,  10,
    11, 10, 6, //10
    1, 2, 6, //6
    2, 3, 7, //5
    3, 4, 8,
    4, 5, 9,
    5, 1, 10, //7
    2,  7, 6,
    3,  8, 7, //4
    4,  9, 8, //3
    5, 10, 9, //8
    1, 6, 10
};

const ofIndexType icoUniqSides[] = {
    2, 1, 0, //0
    5, 4, 0, //1
    4, 3, 0, //2
    4, 9, 8, //3
    3, 8, 7,
    2, 3, 7,
    1, 2, 6, //6
    5, 1, 10, //7
    5, 10, 9, //8
    11, 6,  7, //9
    11, 10, 6, //10
    11, 8,  9, //11
};

struct grabStruct {
    unsigned int camNum;
    ofVec2f point;
};

// first ofVec3f(cam_num, vert.x, vert.y)
const ofVec2f icoGrabSides[] = {
    ofVec2f(0, 1),
    ofVec2f(1, 2),
    ofVec2f(2, 0),
    ofVec2f(0, 5),
    ofVec2f(5, 1),
    ofVec2f(1, 6),
    ofVec2f(6, 2),
	ofVec2f(2, 3),
	ofVec2f(3, 0),
    ofVec2f(0, 4), // 10 sides
    
	ofVec2f(1, 10),
	ofVec2f(10, 9),
	ofVec2f(9, 5),
	ofVec2f(5, 4),
	ofVec2f(4, 3),
	ofVec2f(3, 7),
    ofVec2f(7, 6),
    ofVec2f(6, 10),
    ofVec2f(10,5), // 5 drop to 10
    ofVec2f(10,11),
    ofVec2f(11,6), // 11 sides
    
    ofVec2f(2, 7),
    ofVec2f(7, 8),
    ofVec2f(8,11),
    ofVec2f(11,9),
    ofVec2f(9, 8),
    ofVec2f(8, 3), // 3 drop to 7
    ofVec2f(7,11), //11 drop to 8
    ofVec2f(8, 4),
    ofVec2f(4, 9), // 9 sides
};

const float Verts[] = {
    0.000f,  0.000f,  1.000f,
    0.894f,  0.000f,  0.447f,
    0.276f,  0.851f,  0.447f,
    -0.724f,  0.526f,  0.447f,
    -0.724f, -0.526f,  0.447f,
    0.276f, -0.851f,  0.447f,
    0.724f,  0.526f, -0.447f,
    -0.276f,  0.851f, -0.447f,
    -0.894f,  0.000f, -0.447f,
    -0.276f, -0.851f, -0.447f,
    0.724f, -0.526f, -0.447f,
    0.000f,  0.000f, -1.000f };

const ofVec3f icoVerts[] = {
    ofVec3f(0.000f,  0.000f,  1.000f),
    ofVec3f(0.894f,  0.000f,  0.447f),
    ofVec3f(0.276f,  0.851f,  0.447f),
    ofVec3f(-0.724f,  0.526f,  0.447f),
    ofVec3f(-0.724f, -0.526f,  0.447f),
    ofVec3f(0.276f, -0.851f,  0.447f),
    ofVec3f(0.724f,  0.526f, -0.447f),
    ofVec3f(-0.276f,  0.851f, -0.447f),
    ofVec3f(-0.894f,  0.000f, -0.447f),
    ofVec3f(-0.276f, -0.851f, -0.447f),
    ofVec3f(0.724f, -0.526f, -0.447f),
    ofVec3f(0.000f,  0.000f, -1.000f)
};

const ofVec3f icoIndices[] = {
    ofVec3f(2, 1, 0),
    ofVec3f(3, 2, 0),
    ofVec3f(4, 3, 0),
    ofVec3f(5, 4, 0),
    ofVec3f(1, 5, 0),
    ofVec3f(11, 6,  7),
    ofVec3f(11, 7,  8),
    ofVec3f(11, 8,  9),
    ofVec3f(11, 9,  10),
    ofVec3f(11, 10, 6),
    ofVec3f(1, 2, 6),
    ofVec3f(2, 3, 7),
    ofVec3f(3, 4, 8),
    ofVec3f(4, 5, 9),
    ofVec3f(5, 1, 10),
    ofVec3f(2,  7, 6),
    ofVec3f(3,  8, 7),
    ofVec3f(4,  9, 8),
    ofVec3f(5, 10, 9),
    ofVec3f(1, 6, 10)
};

// ---------------
template <class T>
void writeRaw(ostringstream& out, T data) {
	out.write((char*) &data, sizeof(data));
}

template <class T>
void readRaw(stringstream& stream, T& data) {
	stream.read((char*) &data, sizeof(T));
}
// ---------------

struct udpPoint {
    ofVec3f pos;
    ofVec3f vel;
};

#define UDP_POINTS_ID 1
#define UDP_MAX_POINT 20
#define UDP_MAX_PACKET 1024

//using namespace cv;

class icoUtils {
public:

    static ofMesh get_vertices() {
        ofMesh ico;
        ico.addVertices(&icoVerts[0], 12);
        ico.addIndices(&Faces[0], 60);
//        return ico;
//        ofxMeshUtils::calcNormals(ico);
//        vector<ofVec3f> vertices = ico.getVertices();
        vector<ofVec3f> vertices = ico.getVertices();
        //    ofMatrix4x4 ROTX = ofMatrix4x4::newRotationMatrix(10.9f,1,0,0);
        //    ofMatrix4x4 R = ofMatrix4x4::newRotationMatrix(18,0,0,1);
        ofMatrix4x4 ROTX = ofMatrix4x4::newRotationMatrix(120.0f,1,0,0);
        ofMatrix4x4 R = ofMatrix4x4::newRotationMatrix(54,0,0,1);
        
        ofMatrix4x4 T = ofMatrix4x4::newTranslationMatrix(ofVec3f(0,0,0));

        for (int i=0; i<vertices.size(); i++) {
            ofVec3f transformedVector = vertices[i]*R*ROTX*T;
            ico.setVertex(i, transformedVector);
            //        ico.setVertex(i, ofVec3f(transformedVector.x, transformedVector.y,transformedVector.z));
        }

        return ico;
    }
    
    static void drawMat(cv::Mat& mat, float x, float y, float width, float height) {
		int glType;
        cv::Mat buffer;

        buffer = mat;

		glType = GL_RGB;

		ofTexture tex;
		int w = buffer.cols;
		int h = buffer.rows;
		tex.allocate(w, h, glType);
		tex.loadData(buffer.ptr(), w, h, glType);
		tex.draw(x, y, width, height);
	}
    
    static void readUdp (ofxUDPManager udpConnection, ofVec3f * points, ofVec3f * vels, unsigned int * labels  ) {
        static char message[UDP_MAX_PACKET];
        int messageLength = udpConnection.Receive(message, UDP_MAX_PACKET);
        if(messageLength > 0) {

            stringstream data;
            data.write(message, messageLength);

            unsigned short blockID;
            
            readRaw(data, blockID);
            unsigned short numberBlocks;
            readRaw(data, numberBlocks);

            points = new ofVec3f [numberBlocks];
            vels = new ofVec3f [numberBlocks];
            labels = new unsigned int [numberBlocks];
//            readRaw(data, blockSize);
            
            
            for(int i = 0; i < numberBlocks; i++) {
                readRaw(data, labels[i]);
                readRaw(data, points[i]);
                readRaw(data, vels[i]);
            }
        }

    }
    
    static float trigFunc(int funcNum, float t) {
        float res = 0;
        switch (funcNum) {
            case 0 :
                res = pow(sin(t*PI), 12);
                break;
            case 1 :
                res = sin(tan(t)*pow(sin(t),10));
                break;
            case 2 :
                res = sin(pow(12,sin(t)));
                break;
            case 3 :
                res = sin(t - PI*tan(t)*0.01);
                break;
            case 4 :
                res = sin(t);
                break;
        }
        return res;
    };

};


//    for (i=0; i<N_CAMERAS;i++) {
//        icoMesh.getVertex(<#ofIndexType i#>)
//        //ofVec2f vert1 =cameras[i]->worldToScreen(icoMesh.getVertices()[icoUniqSides[i*3+j]], viewGrid[i]);
//        //ofVec2f vert2 =cameras[i]->worldToScreen(icoMesh.getVertices()[icoUniqSides[i*3+j]], viewGrid[i]);
//        ofVec2f vert1 = triGrabPoints[i][0];
//        ofVec2f vert2 = triGrabPoints[i][1];
//        for (int j=0;j<80;j++) {
//            ofVec2f tmp = vert1.getInterpolated(vert2, j*0.0125f);
//            ofDrawSphere(tmp.x, tmp.x, 1.f);
//            udpImageMat.at<ofVec3f>(row_cntr, j) = sidesImageMat.at<ofVec3f>(tmp.y, tmp.x);
//
//            ofSetColor(sidesGrabImg.getColor(tmp.x, tmp.y));
//            ofSetColor(255);
//            if (j %2 ==0) ofDrawSphere(tmp.x, tmp.x, 1.f);
//            ofColor_<unsigned char> col =  sidesGrabImg.getColor(tmp.x, tmp.y);
//            memcpy(sidesImg.getPixels()[row_cntr*80+j], screenImg.getPixels()[int(tmp.y*screenImg.width+tmp.x)], sizeof(ofPixels));
//            sidesImg.setColor(j, row_cntr, sidesGrabImg.getColor(tmp.x, tmp.y));
//            ofVec3f bgrPixel = sidesImageMat.at<ofVec3f>(tmp.x, tmp.y);
//
//            sidesImg.setColor(j, row_cntr, ofColor(bgrPixel[2],bgrPixel[1],bgrPixel[0]));
//
//        }
//        indexGrabPixels[icoUniqSides[i*3+0]][icoUniqSides[i*3+1]] = row_cntr++;
//        vert1.set(triGrabPoints[i][1]);
//        vert2.set(triGrabPoints[i][2]);
//        int dist = vert1.distance(vert2);
//        for (int j=0;j<80;j++) {
//            ofVec2f tmp = vert1.getInterpolated(vert2, j*0.0125f);
//            udpImageMat.at<ofVec3f>(row_cntr, j) = sidesImageMat.at<ofVec3f>(tmp.y, tmp.x);
//
//            ofSetColor(255);
//            if (j %2 ==0) ofDrawSphere(tmp.x, tmp.x, 1.f);
//        }
//        indexGrabPixels[icoUniqSides[i*3+1]][icoUniqSides[i*3+2]] = row_cntr++;
//        vert1 = triGrabPoints[i][2];
//        vert2 = triGrabPoints[i][0];
//
//        for (int j=0;j<80;j++) {
//            ofVec2f tmp = vert1.getInterpolated(vert2, j*0.0125f);
//            udpImageMat.at<ofVec3f>(row_cntr, j) = sidesImageMat.at<ofVec3f>(tmp.y, tmp.x);
//
//            ofSetColor(255);
//            ofDrawSphere(tmp.x, tmp.x, 1.f);
//        }
//        indexGrabPixels[icoUniqSides[i*3+2]][icoUniqSides[i*3+0]] = row_cntr++;
//    }