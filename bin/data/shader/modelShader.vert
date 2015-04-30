#version 150
precision highp float;

uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 camMvpMatrix;
uniform sampler2DRect modelTransTexture;
uniform sampler2DRect animationTexture;
uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

in vec4  position;
in vec2  texcoord;
in vec3  normal;
in vec4 boneIDs;
in vec4 weights;

out VSOUT
{
    vec2 texCoordVarying;
    vec3 N;
    vec3 L;
    vec3 V;
    vec4 weightCol;
} vsout;

mat4 getMat(sampler2DRect tex, float stepx, float stepy)
{
    float x = stepx * 4.0;
    float y = stepy;
    
    mat4 mat = mat4(texture(tex, vec2((x+0.5), y+0.5)),
                    texture(tex, vec2((x+1.5), y+0.5)),
                    texture(tex, vec2((x+2.5), y+0.5)),
                    texture(tex, vec2((x+3.5), y+0.5)));
    return mat;
}

void main()
{
    mat4 transformMatrix = getMat(modelTransTexture, gl_InstanceID, 0.0);
    
    vec4 mPos = vec4(0.0);
    mPos += weights[0] * (getMat(animationTexture, boneIDs[0], gl_InstanceID) * position);
    mPos += weights[1] * (getMat(animationTexture, boneIDs[1], gl_InstanceID) * position);
    mPos += weights[2] * (getMat(animationTexture, boneIDs[2], gl_InstanceID) * position);
    mPos += weights[3] * (getMat(animationTexture, boneIDs[3], gl_InstanceID) * position);
    vec4 vPos = transformMatrix * mPos;
    
    vsout.N = mat3(modelViewMatrix) * normal;
    vsout.L = light_pos - vPos.xyz;
    vsout.V = -vPos.xyz;
    vsout.texCoordVarying = (textureMatrix * vec4(texcoord.x, texcoord.y, 0, 1)).xy;
    vsout.weightCol = weights;    
    
    gl_Position = camMvpMatrix * vPos;
}