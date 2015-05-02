uniform sampler2DRect diffuseTex;
uniform vec4 globalColor = vec4(1.0);

in PHONGOUT
{
    vec2 texCoordVarying;
    vec3 N;
    vec3 L;
    vec3 V;
} phongIn;

vec3 getFragColor()
{
    vec4 texColor = texture(diffuseTex, phongIn.texCoordVarying) * globalColor;
    vec3 diffuse_albedo = texColor.rgb * 1.6;
    
    vec3 N = normalize(phongIn.N);
    vec3 L = normalize(phongIn.L);
    vec3 V = normalize(phongIn.V);
    vec3 R = reflect(-L, N);
    vec3 diffuse = max(dot(N, L), 0.6) * diffuse_albedo;
    vec3 specular_albedo = vec3(0.0);
    float specular_power = 128.0;
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;
    vec3 finalColor = diffuse + specular;
    
    return finalColor;
}