//
//  Model.cpp
//  emptyExample
//
//  Created by Akira on 4/29/15.
//
//

#include "Model.h"

void Model::setup()
{
    nInstance = 1;
    
    for (int i = 0; i < nInstance; i++)
    {
        animPcts.push_back(ofRandom(1.0));
    }
    
    shader.load("shader/modelShader");
    
    loader.loadModel("model/astroBoy_walk.dae", true);
    loader.setPosition(0, 0, 0);
    loader.setScale(0.5, 0.5, 0.5);
    loader.setRotation(0, 180, 1, 0, 0);
    ofLoadImage(diffuseTex, "model/boy_10.tga");
    modelMatrix = loader.getModelMatrix();
    
    genMeshPieces();
    bindBoneIDAndWeightToAttribute();
    populateEveryAnimationMatrix();
    
    modelTransTexture.allocate(nInstance * 4, 1, GL_RGBA32F, GL_RGBA, GL_FLOAT);
}

void Model::update()
{
    float t = Globals::ELAPSED_TIME;
    
    // model transform
    vector<float> modelMats;
    for(int i = 0; i < nInstance; i++)
    {
        ofVec3f pos = ofVec3f::zero();
        pos.y = -20;
        
        float scale = 4.0;
        
        ofMatrix4x4 mat = ofMatrix4x4::newTranslationMatrix(ofVec3f::zero());
        
        mat.scale(scale, scale, scale);
        mat.translate(pos);
        
        float* ptr = mat.getPtr();
        for(int j = 0; j < matCell; j++)
        {
            modelMats.push_back(ptr[j]);
        }
    }
    modelTransTexture.loadData(&modelMats[0], nInstance * 4, 1, GL_RGBA);
    
    
    // piece transform
    for (auto &ap : animPcts)
    {
        ap += 0.015;
        if (ap > 1.0)
            ap = 0.0;
    }
    
    for (auto &piece : pieces)
    {
        vector<float> animMat;
        
        BoneMatricesPerFrame bmpf = piece.bmfs.at(ofMap(animPcts.at(0),
                                                        0.0, 1.0,
                                                        0, piece.bmfs.size(),
                                                        true));
        for (auto mat : bmpf.matrices)
        {
            float* ptr = mat.getPtr();
            for(int j = 0; j < matCell; j++)
            {
                animMat.push_back(ptr[j]);
            }
        }
        
        piece.instancedAnimTextre.loadData(&animMat[0], nInstance * 4 * maxNBone, 1, GL_RGBA);
    }
}

void Model::draw(ofMatrix4x4 camMvpMatrix)
{
    diffuseTex.bind();
    shader.begin();
    ofPushStyle();
    
    GLuint matLoc = glGetUniformLocation(shader.getProgram(), "camMvpMatrix");
    if (matLoc != -1)
    {
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, camMvpMatrix.getPtr());
    }

    shader.setUniformTexture("modelTransTexture", modelTransTexture, 1);
    
    for (auto p : pieces)
    {
        shader.setUniformTexture("animationTexture", p.instancedAnimTextre, 2);
        shader.setUniformTexture("diffuseTex", diffuseTex, 3);
        
        p.material.begin();
        
        ofEnableBlendMode(p.blendmode);
        
        p.vbo.drawElements(GL_TRIANGLES, p.vbo.getNumIndices());
        
        p.material.end();
    }
    
    ofPopStyle();
    shader.end();
    diffuseTex.unbind();
}

void Model::genMeshPieces()
{
    loader.setPositionForAllAnimations(0.0);
    loader.update();
    for (int i = 0; i < loader.getNumMeshes(); i++)
    {
        MeshPiece mp;
        
        ofxAssimpMeshHelper* meshHelper = &loader.getMeshHelper(i);
        mp.meshHelper = meshHelper;

        ofMesh mesh = loader.getMesh(i);
        mp.vbo = meshHelper->vbo;
        mp.vbo.clearVertices();
        mp.vbo.setVertexData(mesh.getVerticesPointer(), mesh.getNumVertices(), GL_STREAM_DRAW);
        
        ofMaterial material = meshHelper->material;
        mp.material = material;
        
        ofBlendMode blend = meshHelper->blendMode;
        mp.blendmode = blend;
        
        mp.instancedAnimTextre.allocate(nInstance * 4 * maxNBone, 1, GL_RGBA32F, GL_RGBA, GL_FLOAT);
        
        pieces.push_back(mp);
    }
}

void Model::bindBoneIDAndWeightToAttribute()
{
    int maxBone = 4;
    
    for (auto &piece : pieces)
    {
        bool bFoundBone = true;
        
        aiMesh* aimesh = piece.meshHelper->mesh;
        
        // init bone info
        piece.boneInfos.resize(aimesh->mNumVertices);
        for (int i = 0; i < aimesh->mNumVertices; i++)
        {
            piece.boneInfos.at(i).bornID.resize(maxBone);
            piece.boneInfos.at(i).weight.resize(maxBone);
            
            for (int j = 0; j < maxBone; j++)
            {
                piece.boneInfos.at(i).bornID.at(j) = 0;
                piece.boneInfos.at(i).weight.at(j) = 0.0;
            }
        }
        
        // no bone, set weight to 1.0.
        if (aimesh->mNumBones == 0)
        {
            for (auto &bi : piece.boneInfos)
            {
                bi.bornID.at(0) = 0;
                bi.weight.at(0) = 1.0;
            }
            
            bFoundBone = false;
        }
        
        if (bFoundBone)
        {
            // lets populate
            for (int boneID = 0; boneID < aimesh->mNumBones; boneID++)
            {
                for (int weightID = 0; weightID < aimesh->mBones[boneID]->mNumWeights; weightID++)
                {
                    int vertexID = aimesh->mBones[boneID]->mWeights[weightID].mVertexId;
                    float weight = aimesh->mBones[boneID]->mWeights[weightID].mWeight;

                    // Select the 4 largest weights
                    for (int slotID = 0; slotID < maxBone; slotID++)
                    {
                        if (piece.boneInfos.at(vertexID).weight.at(slotID) < weight)
                        {
                            for (int shuff = 3; shuff > slotID; shuff--)
                            {
                                piece.boneInfos.at(vertexID).weight.at(shuff) = piece.boneInfos.at(vertexID).weight.at(shuff-1);
                                piece.boneInfos.at(vertexID).bornID.at(shuff) = piece.boneInfos.at(vertexID).bornID.at(shuff-1);
                            }
                            piece.boneInfos.at(vertexID).weight.at(slotID) = weight;
                            piece.boneInfos.at(vertexID).bornID.at(slotID) = boneID;
                            break;
                        }
                    }
                }
            }
        }
        
        // bind attribute
        vector<float> boneIDs;
        vector<float> weights;
        for (auto bi : piece.boneInfos)
        {
            for (int i = 0; i < maxBone; i++)
            {
                boneIDs.push_back(bi.bornID.at(i));
                weights.push_back(bi.weight.at(i));
            }
        }
        
        int boneIDLoc = shader.getAttributeLocation("boneIDs");
        piece.vbo.setAttributeData(boneIDLoc,
                                               &boneIDs[0],
                                               4,
                                               boneIDs.size(),
                                               GL_STATIC_DRAW,
                                               sizeof(ofVec4f));
        
        int weightLoc = shader.getAttributeLocation("weights");
        piece.vbo.setAttributeData(weightLoc,
                                               &weights[0],
                                               4,
                                               weights.size(),
                                               GL_STATIC_DRAW,
                                               sizeof(ofVec4f));
        
//        for (auto bi : piece.boneInfos)
//        {
//            bi.dump();
//        }
    }
}

void Model::populateEveryAnimationMatrix()
{
    float step = 0.01;
    for (float i = 0; i < 1.0; i += step)
    {
        loader.setPositionForAllAnimations(i);
        loader.update();
        
        for (auto &piece : pieces)
        {
            aiMesh* aimesh = piece.meshHelper->mesh;
            
            vector<aiMatrix4x4> aiBoneMatrices(aimesh->mNumBones);
            
            for (int a = 0; a < aimesh->mNumBones; a++)
            {
                aiBone* bone = aimesh->mBones[a];
                aiNode* node = loader.getAssimpScene()->mRootNode->FindNode(bone->mName);
                aiBoneMatrices[a] = bone->mOffsetMatrix;
                
                aiNode* tempNode = node;
                while (tempNode)
                {
                    aiBoneMatrices[a] = tempNode->mTransformation * aiBoneMatrices[a];
                    tempNode = tempNode->mParent;
                }
            }
            
            vector<ofMatrix4x4> boneMatrices;
            for (auto aim : aiBoneMatrices)
            {
                ofMatrix4x4 ofmat = ofMatrix4x4::newTranslationMatrix(ofVec3f::zero());
                
                aiMatrix4x4 temp = aim;
                temp.Transpose();
                ofmat.set(temp.a1, temp.a2, temp.a3, temp.a4,
                          temp.b1, temp.b2, temp.b3, temp.b4,
                          temp.c1, temp.c2, temp.c3, temp.c4,
                          temp.d1, temp.d2, temp.d3, temp.d4);
                
                boneMatrices.push_back(ofmat);
            }
            
            while (boneMatrices.size() < 4)
            {
                ofMatrix4x4 ofmat = ofMatrix4x4::newTranslationMatrix(ofVec3f::zero());
                boneMatrices.push_back(ofmat);
            }
            
            BoneMatricesPerFrame bmf;
            bmf.matrices = boneMatrices;
            piece.bmfs.push_back(bmf);
        }
    }
    
    loader.setPositionForAllAnimations(0);
    loader.update();
}




