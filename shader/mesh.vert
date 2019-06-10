attribute highp vec3 posAttr;
attribute highp vec3 normAttr;
attribute highp vec2 uvAttr;
varying lowp vec4 col;
uniform highp mat4 viewMatrix;
uniform highp mat4 transMatrix;

void main() {
   col = vec4(abs(normAttr),1);
   gl_Position = viewMatrix * transMatrix * vec4(posAttr,1);
}