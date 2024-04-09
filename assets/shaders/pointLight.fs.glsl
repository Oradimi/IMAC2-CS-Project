#version 330

uniform sampler2D uTexture;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;
uniform float uTime;

in vec3 vUV;
in vec3 vPosition_vs;
in vec3 vNormal_vs;

out vec4 fFragColor;

vec3 blinnPhong() {
    float distanceLightFragment = distance(uLightPos_vs, vPosition_vs);
    vec3 nLightPosToPosition_vs = normalize(uLightPos_vs - vPosition_vs);
    vec3 nNormal_vs = normalize(vNormal_vs);
    vec3 nPosition_vs = normalize(-vPosition_vs);
    
    vec3 halfwayVector = normalize(nLightPosToPosition_vs + nPosition_vs);

    float diffuseTerm = max(dot(nLightPosToPosition_vs, nNormal_vs), 0.0);
    vec3 diffuseColor = uKd * diffuseTerm;

    float specularTerm = pow(max(dot(nNormal_vs, halfwayVector), 0.0), uShininess);
    vec3 specularColor = uKs * specularTerm;

    float attenuation = 1.0 / (distanceLightFragment * distanceLightFragment);
    
    return uLightIntensity * attenuation * (diffuseColor + specularColor);
}

void main()
{
    vec4 textureColor = texture(uTexture, vUV.xy);
    fFragColor = vec4(textureColor.xyz * blinnPhong(), 1.);
}
