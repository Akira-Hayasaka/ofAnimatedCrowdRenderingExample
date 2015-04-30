//
//  MeshPiece.h
//  emptyExample
//
//  Created by Akira on 4/29/15.
//
//

#ifndef __emptyExample__MeshPiece__
#define __emptyExample__MeshPiece__

#include "ofMain.h"
#include "ofxAssimpMeshHelper.h"

class BoneInfo
{
public:
    
    void dump()
    {
        ofLog() << "";
        for (int i = 0; i < bornID.size(); i++)
        {
            ofLog() << "bornID = " << bornID.at(i) << " weight = " << weight.at(i);
        }
    }
    
    vector<int> bornID;
    vector<float> weight;
};

class BoneMatricesPerFrame
{
public:
    vector<ofMatrix4x4> matrices;
};

class MeshPiece
{
public:
    
    ofTexture instancedAnimTextre; // <= take all instance's anim mat of this frame
    
    ofVbo vbo;
    ofMaterial material;
    ofBlendMode blendmode;
    vector<BoneInfo> boneInfos;
    vector<BoneMatricesPerFrame> bmfs;
    ofxAssimpMeshHelper* meshHelper;
};

#endif /* defined(__emptyExample__MeshPiece__) */
