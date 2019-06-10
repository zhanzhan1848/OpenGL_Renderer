#pragma once
#include "Modle.h"
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class Skybox;
class Skydome;
using SkyboxPtr = std::shared_ptr<Skybox>;
using SkydomePtr = std::shared_ptr<Skydome>;

class Skybox:protected QOpenGLFunctions
{
public:
    static SkyboxPtr create();
    Skybox();
    ~Skybox();
    QOpenGLTexture *texture;
    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO;
    QOpenGLShaderProgram *shader;
    GLuint m_posAttr;
    GLuint viewMatrix;
    QList<QString> texPath;
    void initCubeTexture();
    void draw(QMatrix4x4 matrix);
};

class Skydome
{
public:
    static SkydomePtr create();
    Skydome();
    ~Skydome();
    void setImage(std::string &texPath);
    QOpenGLTexture *texture;
    GLuint textureID;
    std::string path;
    modlePtr dome;
    meshPtr mesh;
    void initMaterial();
    void draw(QMatrix4x4 matrix);
};
