#version 330 core
uniform samplerCube skybox;
varying highp vec3 TexCoords;

void main() {
    vec3 c =  normalize(TexCoords);
    gl_FragColor = texture(skybox,c);
}