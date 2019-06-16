#version 330 core
varying highp vec2 TexCoords;
uniform sampler2D skydome;

void main() {
   gl_FragColor = texture(skydome,TexCoords);
}