#version 150

#pragma include "./shading/phong.frag"

out vec4 fragColor;

void main()
{
    fragColor = vec4(getFragColor(), 1.0);
}