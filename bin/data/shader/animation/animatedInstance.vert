precision highp float;

uniform sampler2DRect animationTexture;

in vec4 boneIDs;
in vec4 weights;

mat4 getMat(sampler2DRect tex, float stepx, float stepy)
{
    float x = stepx * 4.0f;
    float y = stepy;
    
    mat4 mat = mat4(texture(tex, vec2(x+0.5f, y + 0.5f)),
                    texture(tex, vec2(x+1.5f, y + 0.5f)),
                    texture(tex, vec2(x+2.5f, y + 0.5f)),
                    texture(tex, vec2(x+3.5f, y + 0.5f)));
    return mat;
}

vec4 getAnimatedVertPos(vec4 vertPos)
{
    vec4 animPos = vec4(0.0f);
    animPos += weights[0] * (getMat(animationTexture, boneIDs[0], gl_InstanceID) * vertPos);
    animPos += weights[1] * (getMat(animationTexture, boneIDs[1], gl_InstanceID) * vertPos);
    animPos += weights[2] * (getMat(animationTexture, boneIDs[2], gl_InstanceID) * vertPos);
    animPos += weights[3] * (getMat(animationTexture, boneIDs[3], gl_InstanceID) * vertPos);
    return animPos;
}