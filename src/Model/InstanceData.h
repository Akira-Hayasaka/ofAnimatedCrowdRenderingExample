//
//  InstanceData.h
//  emptyExample
//
//  Created by Akira on 4/30/15.
//
//

#ifndef __emptyExample__InstanceData__
#define __emptyExample__InstanceData__

#include "ofMain.h"
#include "Globals.h"

class InstanceData
{
public:
    
    void setup(int idx);
    void update();
    ofMatrix4x4 getTransform();
    
    int idx;
    
    float animPct;
    ofVec3f pos;
    ofVec3f axis;
    ofMatrix4x4 mat;
};

#endif /* defined(__emptyExample__InstanceData__) */
