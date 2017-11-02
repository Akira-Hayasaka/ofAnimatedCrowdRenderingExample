#version 150

#pragma include "./animation/animatedInstance.vert"
#pragma include "./shading/phong.vert"

uniform sampler2DRect modelTransTexture;
uniform mat4 camMvpMatrix;

in vec4  position;

void main()
{
    mat4 transformMatrix = getMat(modelTransTexture, gl_InstanceID, 0.0);
    vec4 vPos = transformMatrix * getAnimatedVertPos(position);
    
    setPhongOut(vPos);
    
    gl_Position = camMvpMatrix * vPos;
}