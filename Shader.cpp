#include "Shader.h"


shaderPtr Shader::Create(std::string &vert, std::string &frag)
{
    shaderPtr s{new Shader(vert,frag)};
    return s;
}

Shader::Shader(std::string &vert, std::string &frag)
{
    addShaderFromSourceFile(QOpenGLShader::Vertex,QString::fromStdString(vert));
    addShaderFromSourceFile(QOpenGLShader::Fragment,QString::fromStdString(frag));
    //create();
    link();
    bind();
    posAttr = attributeLocation("posAttr");
    normAttr = attributeLocation("normAttr");
    uvAttr = attributeLocation("uvAttr");
    viewMatrix = uniformLocation("viewMatrix");
    transMatrix = uniformLocation("transMatrix");
    release();
}

void Shader::setMatrix(QMatrix4x4 view,QMatrix4x4 trans)
{
    setUniformValue(viewMatrix, view);
    setUniformValue(transMatrix, trans);
}

void Shader::setViewMatrix(QMatrix4x4 view)
{
    setUniformValue(viewMatrix, view);
}

