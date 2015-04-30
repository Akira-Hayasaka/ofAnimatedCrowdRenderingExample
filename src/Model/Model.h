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

class Model
{
public:
    
    void setup();
    void update();
    void draw(ofMatrix4x4 camMvpMatrix);
    
private:
    
    void genMeshPieces();
    void bindBoneIDAndWeightToAttribute();
    void populateEveryAnimationMatrix();
    
    const int maxNBone = 100;
    const int matCell = 16;
    
    int nInstance;
    
    ofTexture modelTransTexture; // <= take all instance's mat of this frame
    
    ofxAssimpModelLoader loader;
    ofMatrix4x4 modelMatrix;
    ofTexture diffuseTex;
    vector<MeshPiece> pieces;
    ofShader shader;
    
    vector<float> animPcts;
};

#endif /* defined(__emptyExample__Model__) */
