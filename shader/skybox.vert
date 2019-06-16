attribute highp vec3 posAttr;
uniform highp mat4 viewMatrix;
//uniform mat4 projMatrix;
varying highp vec3 TexCoords;
 
void main(){
  TexCoords = posAttr/10.0;
  vec4 pos =  viewMatrix * vec4(posAttr, 1.0f);
  gl_Position = pos.xyww;
}
