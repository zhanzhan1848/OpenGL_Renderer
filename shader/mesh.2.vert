#version 330 core

attribute highp vec3 posAttr;
attribute highp vec3 normAttr;
attribute highp vec2 uvAttr;
//varying highp vec4 col;

varying highp vec3 normal;
varying highp mat4 viewM;
varying highp mat4 projM;
varying highp vec2 uv;
varying highp vec3 worldPos;

uniform highp mat4 projMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 transMatrix;


const float roughness = 0.5;
const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
 
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
 
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
 
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
 
    return nom / denom;
}

void main() {
   //col = vec4(abs(normAttr),1);
   normal = transMatrix * normAttr;
   viewM = viewMatrix;
   projM = projMatrix;
   uv = uvAttr;
   worldPos = vec3(transMatrix * vec4(posAttr, 1.0));
   gl_Position = projMatrix * viewMatrix * vec4(worldPos,1.0);
}

