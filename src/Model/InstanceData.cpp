//
//  InstanceData.cpp
//  emptyExample
//
//  Created by Akira on 4/30/15.
//
//

#include "InstanceData.h"

void InstanceData::setup(int idx)
{
    this->idx = idx;
    animPct = ofRandom(1.0);
    
    int xbegin = -500;
    
    int zbegin = -500;
    if (idx % 2 == 0)
        zbegin = -550;
    pos.x = xbegin + (idx * 5);
    pos.y = -15;
    pos.z = zbegin + (idx * 5);
}

void InstanceData::update()
{
//    float t = Globals::ELAPSED_TIME;
//    
//    pos.x = ofNoise(0.001 * t, idx + 0) * 100 -50;
//    pos.y = ofNoise(0.001 * t, idx + 10) * 100 -50;
//    pos.z = ofNoise(0.001 * t, idx + 20) * 100 -50;
//    
//    float angle = ofNoise(0.05 * t, idx + 60) * 360;
//    
//    axis.x = ofNoise(0.02 * t, idx + 30);
//    axis.y = ofNoise(0.02 * t, idx + 40);
//    axis.z = ofNoise(0.02 * t, idx + 50);
//    
//    float scale = ofNoise(0.02 * t, idx + 70) * 2.0 + 0.5;
//    
    mat = ofMatrix4x4::newTranslationMatrix(ofVec3f());
//
//    mat.rotate(angle, axis.x, axis.y, axis.z);
//    mat.scale(scale, scale, scale);
    mat.translate(pos);
    
    animPct += 0.015;
    if (animPct > 1.0)
        animPct = 0.0;
}

ofMatrix4x4 InstanceData::getTransform()
{
    return mat;
}