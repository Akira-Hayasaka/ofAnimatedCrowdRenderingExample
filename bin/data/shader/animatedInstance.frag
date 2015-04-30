#version 150

uniform sampler2DRect diffuseTex;
uniform vec4 globalColor = vec4(1.0);

in VSOUT
{
    vec2 texCoordVarying;
    vec3 N;
    vec3 L;
    vec3 V;
    vec4 weightCol;
} fsin;

out vec4 fragColor;

void main()
{
    vec4 texColor = texture(diffuseTex, fsin.texCoordVarying) * globalColor;
    vec3 diffuse_albedo = texColor.rgb * 1.6;
    
    vec3 N = normalize(fsin.N);
    vec3 L = normalize(fsin.L);
    vec3 V = normalize(fsin.V);
    vec3 R = reflect(-L, N);
    vec3 diffuse = max(dot(N, L), 0.6) * diffuse_albedo;
    vec3 specular_albedo = vec3(0.0);
    float specular_power = 128.0;
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;
    
    vec3 finalColor = diffuse + specular;
    
    fragColor = vec4(finalColor, 1.0);
//    fragColor = vec4(fsin.weightCol.rgb, 1.0);
}