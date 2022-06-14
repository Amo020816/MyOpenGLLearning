#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uTrInvModel;

out vec3 fNormal;
out vec3 fFragPos;
out vec2 fTexCoord;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    fFragPos = vec3( uModel * vec4(aPos, 1.0));
    fNormal =  mat3(uTrInvModel) * aNormal;
    fTexCoord = aTexCoord;
}