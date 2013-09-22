//
//  ofxMesh.cpp
//  icoExplore
//
//  Created by Tim Tavli on 20.09.13.
//
//

#include "ofxMesh.h"

ofxMesh::ofxMesh() : ofMesh(){
    ;;
}

//--------------------------------------------------------------
ofxMesh::ofxMesh(ofPrimitiveMode mode, const vector<ofVec3f>& verts){
	setMode(mode);
	addVertices(verts);
}

//--------------------------------------------------------------
ofxMesh::~ofxMesh(){
    
}

void ofxMesh::drawInMesh(ofMesh mesh) {
    if(getNumVertices()==0) return;
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glEnable      ( GL_STENCIL_TEST );
    
    glStencilFunc ( GL_ALWAYS, 1, 0 );
    glStencilOp   ( GL_KEEP, GL_KEEP, GL_REPLACE );
    
    ofGetCurrentRenderer()->draw(*this,OF_MESH_FILL,ofMesh::usingColors(),ofMesh::usingTextures(),ofMesh::usingNormals());

    ofSetColor(0, ofRandomuf(), ofRandomuf());
    ofSetColor(0, 255.0f * sin(ofGetFrameNum()*.05f), 255.f*cos(ofGetFrameNum()*.05f));
    
    glStencilFunc ( GL_ALWAYS, 2, 0 );
    glStencilOp   ( GL_KEEP, GL_KEEP, GL_REPLACE );
    
    mesh.draw();
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glStencilFunc(GL_EQUAL, 1, 255);
    
    mesh.draw();
    
    glDisable     ( GL_STENCIL_TEST );

}

ofxMesh ofxMesh::xbox( float width, float height, float depth, int resX, int resY, int resZ ) {
    // mesh only available as triangles //
    ofxMesh mesh;
    mesh.setMode( OF_PRIMITIVE_TRIANGLES );
    
    // halves //
    float halfW = width * .5f;
    float halfH = height * .5f;
    float halfD = depth * .5f;
    
    ofVec3f vert;
    ofVec2f texcoord;
    ofVec3f normal;
    int vertOffset = 0;
    
    // TRIANGLES //
    
    // Front Face //
    normal.set(0, 0, 1);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * width - halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Right Side Face //
    normal.set(1, 0, 0);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * -depth + halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    // Left Side Face //
    normal.set(-1, 0, 0);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = -halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * depth - halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Back Face //
    normal.set(0, 0, -1);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * -width + halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = -halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    
    // Top Face //
    normal.set(0, -1, 0);
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = -halfH;
            vert.z = texcoord.y * depth - halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Bottom Face //
    normal.set(0, 1, 0);
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = halfH;
            vert.z = texcoord.y * -depth + halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    
    
    return mesh;
}