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

class mesh;
class modle;
using meshPtr = std::shared_ptr<mesh>;
using modlePtr = std::shared_ptr<modle>;

struct vertex {
    size_t index = -1;
    std::vector<float> position;
    std::vector<float>  normal;
    std::vector<float>  uv;
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

    //QOpenGLExtraFunctions * f;
    QOpenGLShaderProgram *shader;
    GLuint m_posAttr;
    GLuint m_normAttr;
    GLuint m_uvAttr;
    GLuint viewMatrix;
    GLuint transMatrix;

    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO,EBO;

    QMatrix4x4 transform;
    void setup();
    void draw(QMatrix4x4 matrix);
    void clearData();

};

class modle {
public:
    static modlePtr create(std::string &objPath);
    modle(std::string &objPath);
    ~modle();
    void setObj(std::string &objPath);
    void draw(QMatrix4x4 matrix);
    std::vector<meshPtr> data;
private:
    std::string path;

    void load();
    void initBuffer();
};

