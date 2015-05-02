#version 150

#pragma include "./animatedInstance.glslinc"

uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 camMvpMatrix;
uniform sampler2DRect modelTransTexture;

uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

in vec4  position;
in vec2  texcoord;
in vec3  normal;

out VSOUT
{
    vec2 texCoordVarying;
    vec3 N;
    vec3 L;
    vec3 V;
    vec4 weightCol;
} vsout;

void main()
{
    mat4 transformMatrix = getMat(modelTransTexture, gl_InstanceID, 0.0);
    
    vec4 vPos = transformMatrix * getAnimatedVertPos(position);
    
    vsout.N = mat3(modelViewMatrix) * normal;
    vsout.L = light_pos - vPos.xyz;
    vsout.V = -vPos.xyz;
    vsout.texCoordVarying = (textureMatrix * vec4(texcoord.x, texcoord.y, 0, 1)).xy;
    vsout.weightCol = weights;
    
    gl_Position = camMvpMatrix * vPos;
}