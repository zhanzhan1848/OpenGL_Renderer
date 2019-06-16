#version 330 core

// in mat4 viewM;
// in mat4 projM;
in vec3 normal;
in vec3 position;
in vec3 camPos;
in vec2 uv;
uniform samplerCube skybox;
//uniform vec4 rdSeed;

const float roughness = 0.5;
const float PI = 3.14159265359;
float rdCnt = 99;

float RandXY(float x, float y)
{
     return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}

float rand()
{
    float a = RandXY(uv.x , position.x );
    float b = RandXY(position.y, uv.y);
    float c = RandXY(rdCnt++, position.z);
    float d = RandXY(normal.x, a);
    float e = RandXY(b, c);
    float f = RandXY(d, e);

    return f;
}

vec3 tangent2world(vec3 vec, vec3 norm) 
{
  vec3 up = abs(norm.y) < 0.999 ? vec3(0, 1, 0) : vec3(0, 0, 1);
  vec3 x = normalize(cross(up, norm));
  vec3 z = cross(norm, x);
  return x*vec.x + norm*vec.y + z*vec.z;
}


vec3 sample_GGX(float randPhi, float randTheta, float rough) 
{
  float m = rough * rough;
  float m2 = m*m;

  float phi = PI * 2 * randPhi;
  float cosTheta = sqrt((1 - randTheta) / (1 + (m2 - 1)*randTheta));
  float sinTheta = sqrt(1 - cosTheta*cosTheta);

  return vec3(sinTheta * cos(phi),cosTheta,sinTheta * sin(phi));
}


//=================BRDF=============================
vec4 refractBRDF(vec3 pos,vec3 cameraPos,vec3 norm)
{
   vec3 micronormal = vec3(0);
   for(int i=0;i<100;++i)
   {
      float a = rand();
      float b = rand();
      micronormal += tangent2world(sample_GGX(a,b,roughness), norm);
   }
   micronormal /= 100.0;

   float ratio = 1.00 / 1.52;
   vec3 I = normalize(pos - cameraPos);
   vec3 R = refract(I, micronormal, ratio);
   return vec4(texture(skybox, R).xyz, 1.0);
}

vec4 reflectBRDF(vec3 pos,vec3 cameraPos,vec3 norm)
{
   vec3 micronormal = vec3(0);
   for(int i=0;i<100;++i)
   {
      float a = rand();
      float b = rand();
      micronormal += tangent2world(sample_GGX(a,b,roughness), norm);
   }
   micronormal /= 100.0;

   vec3 I = normalize(pos - cameraPos);
   vec3 R = reflect(I, micronormal);
   return vec4(texture(skybox,R).xyz,1);
}

void main() 
{
   gl_FragColor = refractBRDF(position,camPos,normal);
}