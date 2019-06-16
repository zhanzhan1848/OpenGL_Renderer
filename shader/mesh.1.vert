#version 330 core
attribute highp vec3 posAttr;
attribute highp vec3 normAttr;
attribute highp vec2 uvAttr;
attribute vec3 cameraPos;


// out mat4 viewM;
// out mat4 projM;
out vec3 normal;
out vec3 position;
out vec3 camPos;
out vec2 uv;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 transMatrix;


void main() {
   camPos = cameraPos;
   uv = uvAttr;
   normal = mat3(transpose(inverse(transMatrix))) * normAttr;
   //viewM = viewMatrix;
   //projM = projMatrix;
   position = vec3(transMatrix * vec4(posAttr, 1.0));
   gl_Position = projMatrix * viewMatrix * vec4(position,1);
}