#version 150

#pragma include "./shading/phong.frag"

out vec4 fragColor;

void main()
{
    fragColor = vec4(getPhongColor(), 1.0);
}