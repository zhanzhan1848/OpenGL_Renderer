#pragma once
#include <QOpenGLShaderProgram>
#include <memory>
#include <iostream>

class Shader;
using shaderPtr = std::shared_ptr<Shader>;

class Shader:public QOpenGLShaderProgram
{
public:
    static shaderPtr Create(std::string &vert,std::string &frag);
    Shader(std::string &vert,std::string &frag);
    GLuint posAttr;
    GLuint normAttr;
    GLuint uvAttr;
    GLuint projMatrix;
    GLuint viewMatrix;
    GLuint transMatrix;
    void setMatrix(QMatrix4x4 projM,QMatrix4x4 viewM,QMatrix4x4 trans);
    void setViewMatrix(QMatrix4x4 viewM);
};
