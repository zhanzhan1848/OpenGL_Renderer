//
// Created by arno on 19-6-7.
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <Shader.h>

class mesh;
class model;
using meshPtr = std::shared_ptr<mesh>;
using modelPtr = std::shared_ptr<model>;

struct vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D uv;
};

struct triangle{
    //std::vector<vertex> vertices;
    vertex pa;
    vertex pb;
    vertex pc;
};

class mesh:protected QOpenGLFunctions
{
public:
    static meshPtr create();
    mesh();
    ~mesh();
    std::string name;
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<size_t> posElements;
    std::vector<size_t> norElements;
    std::vector<size_t> uvElements;
    std::vector<triangle> data;
    //QOpenGLExtraFunctions * f;
    shaderPtr shader;

    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO,EBO;

    QMatrix4x4 transform;
    void setup();
    void draw(QMatrix4x4 projM,QMatrix4x4 viewM);
    void clearData();
    void setShader(shaderPtr s);
};

class model {
public:
    static modelPtr create(std::string &objPath);
    model(std::string &objPath);
    ~model();
    void setObj(std::string &objPath);
    void draw(QMatrix4x4 projM,QMatrix4x4 viewM);
    std::vector<meshPtr> data;

    void setShader(shaderPtr s);
private:
    std::string path;

    void load();
    void initBuffer();
};

