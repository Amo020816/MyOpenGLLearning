#version 460 core
out vec4 FragColor;

in vec3 fNormal;
in vec3 fFragPos;
in vec2 fTexCoord;


uniform sampler2D uCubeTexture;
// light cube
uniform vec3 uLightPos;
uniform vec3 uLightColor;


void main()
{
    // diffuse
    vec3 normal_vec = normalize(fNormal);
    vec3 light_dir = normalize(uLightPos- fFragPos);
    float diff_coe = max(dot(normal_vec, light_dir), 0.0);
    vec3 diffuse = diff_coe * uLightColor;

    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * uLightColor;
    vec3 result = ambient + diffuse;
    FragColor = vec4(result , 1.0) * texture(uCubeTexture, fTexCoord);
}