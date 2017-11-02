//
//  Model.h
//  emptyExample
//
//  Created by Akira on 4/29/15.
//
//

#ifndef __emptyExample__Model__
#define __emptyExample__Model__

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxAssimpUtils.h"
#include "Globals.h"
#include "MeshPiece.h"
#include "InstanceData.h"

class Model
{
public:
    
    void setup();
    void update();
    void draw(ofMatrix4x4 camMvpMatrix);
    
    void drawDebug();
    
private:
    
    void genMeshPieces();
    void bindBoneIDAndWeightToAttribute();
    void populateEveryAnimationMatrix();
    
    const int maxNBone = 100;
    const int matCell = 16;
    
    int nInstance;
    vector<InstanceData> instanceDatas;
    
    ofTexture modelTransTexture; // <= take all instance's mat of this frame
    ofTexture diffuseTex;
    ofxAssimpModelLoader loader;
    ofMatrix4x4 modelMatrix;
    
    vector<MeshPiece> pieces;
    ofShader shader;
};

#endif /* defined(__emptyExample__Model__) */
