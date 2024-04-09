#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 vTexCoords;
in vec3 vPosition_vs;
in vec3 vNormal_vs;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

out vec4 fFragColor;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = vPosition_vs;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(vNormal_vs);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(texture_diffuse1, vTexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(texture_specular1, vTexCoords).r;

    fFragColor = vec4(vPosition_vs.xyz, 1.);
}  