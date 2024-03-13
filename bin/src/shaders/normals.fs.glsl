#version 330

in vec3 vNormal_vs;

out vec4 fFragColor;

void main()
{
    fFragColor = vec4(normalize(vNormal_vs), 1.);
}
