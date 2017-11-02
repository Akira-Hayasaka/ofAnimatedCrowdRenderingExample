uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

in vec3  normal;
in vec2  texcoord;

out PHONGOUT
{
    vec2 texCoordVarying;
    vec3 N;
    vec3 L;
    vec3 V;
} phongOut;

void setPhongOut(vec4 vPos)
{
    phongOut.N = mat3(modelViewMatrix) * normal;
    phongOut.L = light_pos - vPos.xyz;
    phongOut.V = -vPos.xyz;
    phongOut.texCoordVarying = (textureMatrix * vec4(texcoord.x, texcoord.y, 0, 1)).xy;
}
