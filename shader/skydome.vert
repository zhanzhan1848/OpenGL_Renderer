attribute highp vec3 posAttr;
attribute highp vec3 normAttr;
attribute highp vec2 uvAttr;
varying lowp vec4 col;
uniform highp mat4 viewMatrix;
uniform highp mat4 transMatrix;

varying highp vec2 TexCoords;

void main() {
    TexCoords = uvAttr;
    col = vec4(abs(normAttr),1);
    vec4 pos = viewMatrix * transMatrix * vec4(posAttr,1);
    gl_Position = pos.xyww;
}
