#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 vPosition_vs;
out vec3 vNormal_vs;
out vec2 vTexCoords;
out vec3 vUV;

void main()
{
    vUV = vec3(aVertexTexCoords, 1.);
    vec4 vertexPosition = vec4(aVertexPosition, 1.);
    vec4 vertexNormal = vec4(aVertexNormal, 0.);

    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = normalize(vec3(uNormalMatrix * vertexNormal));
    vTexCoords = aVertexTexCoords;

    gl_Position = uMVPMatrix * vertexPosition;
}