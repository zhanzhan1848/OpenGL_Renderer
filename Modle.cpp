//
// Created by arno on 19-6-7.
//

#include "Modle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShader>
#include <QMatrix4x4>
#include <QFileInfo>
#include <globalvar.h>

modle::modle(std::string &objPath) {
    path = objPath;
    load();
}

void modle::setObj(std::string &objPath) {
    remove();
    path = objPath;
    load();
}

void modle::load() {
    std::ifstream in(path.c_str(), std::ifstream::in);

    if (in.is_open()) {
        std::string line;
        mesh* currentMesh = new mesh();
        data.push_back(currentMesh);

        int idx = 0;
        size_t offset_pos = 0;
        size_t offset_normal = 0;
        size_t offset_uv = 0;
        while (std::getline(in,line))
        {
            QString l = QString::fromStdString(line);
            QStringList dt = l.split(" ");
            int len = dt.length();
            if(len<1) continue;
            QString first = dt.first();
            dt.removeFirst();

            //qDebug()<<dt;
            if(first=="o")
            {
                offset_pos += currentMesh->positions.size()/3;
                offset_normal += currentMesh->normals.size()/3;
                offset_uv += currentMesh->uvs.size()/2;
                if(idx!=0)
                {
                    currentMesh = new mesh();
                    data.push_back(currentMesh);
                 }
                currentMesh->name = dt.join("").toStdString();
                //std::cout<<currentMesh->name<<std::endl;
                ++idx;
            }
            else if(first=="v")
            {
                currentMesh->positions.push_back(dt.at(0).toFloat());
                currentMesh->positions.push_back(dt.at(1).toFloat());
                currentMesh->positions.push_back(dt.at(2).toFloat());
                //qDebug()<<QString::fromStdString(currentMesh->name);
            }
            else if(first=="vn")
            {
                currentMesh->normals.push_back(dt.at(0).toFloat());
                currentMesh->normals.push_back(dt.at(1).toFloat());
                currentMesh->normals.push_back(dt.at(2).toFloat());
            }
            else if(first == "vt")
            {
                currentMesh->uvs.push_back(dt.at(0).toFloat());
                currentMesh->uvs.push_back(dt.at(1).toFloat());
            }
            else if(first=="f")
            {
                if(dt.length()==3)
                {
                    for(int j=0;j<3;++j)
                    {
                        QString i = dt.at(j);
                        QStringList dt2 = i.split("/");
                        currentMesh->posElements.push_back(dt2.at(0).toUInt()-1-offset_pos);
                        currentMesh->uvElements.push_back(dt2.at(1).toUInt()-1-offset_uv);
                        currentMesh->norElements.push_back(dt2.at(2).toUInt()-1-offset_normal);
                    }
                }

                else if(dt.length()==4)
                {
                    std::vector<int>index = {0,1,2,2,3,0};
                    for(int j=0;j<2;++j)
                    {
                        for(int k=0;k<3;++k)
                        {
                            int dx = k + j*3;
                            QString i = dt.at(index[dx]);
                            QStringList dt2 = i.split("/");
                            currentMesh->posElements.push_back(dt2.at(0).toUInt()-1-offset_pos);
                            currentMesh->uvElements.push_back(dt2.at(1).toUInt()-1-offset_uv);
                            currentMesh->norElements.push_back(dt2.at(2).toUInt()-1-offset_normal);
                        }
                    }
                }
            }
        }

        in.close();
    }

    //qDebug()<<data.size();
    for(mesh* m:data)
    {
        m->setup();
    }

}

void mesh::setup()
{
    if(posElements.size()==0) return;
    std::vector<float> data;

    for(int i=0;i<int(posElements.size());++i)
    {
        data.push_back(positions[posElements[i]*3]);
        data.push_back(positions[posElements[i]*3+1]);
        data.push_back(positions[posElements[i]*3+2]);

        data.push_back(normals[norElements[i]*3]);
        data.push_back(normals[norElements[i]*3+1]);
        data.push_back(normals[norElements[i]*3+2]);

        data.push_back(uvs[uvElements[i]*2]);
        data.push_back(uvs[uvElements[i]*2+1]);
    }

    qDebug()<<data.size();
    qDebug()<<posElements.size();

    //bind buffer
    shader->bind();
    VAO.bind();
    VBO.bind();
    //EBO.bind();

    //VBO.allocate(&data,int(data.size())*sizeof(GLfloat));
    //VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    EBO.allocate(&posElements,int(posElements.size())*sizeof(GLfloat));

    //allocate VBO
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

    //data to shader
    int stride = 8*sizeof(GLfloat);
    shader->enableAttributeArray(m_posAttr);
    shader->setAttributeBuffer(m_posAttr,GL_FLOAT,0,3,stride);
    shader->enableAttributeArray(m_normAttr);
    shader->setAttributeBuffer(m_normAttr,GL_FLOAT,3*sizeof(GLfloat),3,stride);
    shader->enableAttributeArray(m_uvAttr);
    shader->setAttributeBuffer(m_uvAttr,GL_FLOAT,6*sizeof(GLfloat),2,stride);

    VAO.release();
    VBO.release();
    shader->release();
}

void mesh::draw(QMatrix4x4 matrix) {
    if(posElements.size()==0) return;

    //setup shader
    shader->bind();
    shader->setUniformValue(viewMatrix, matrix);
    shader->setUniformValue(transMatrix, transform);

    // Draw mesh
    VAO.bind();
    glDrawArrays(RENDER_MODE, 0, int(posElements.size()));
    VAO.release();
    //EBO.bind()
    //glDrawElements(GL_TRIANGLES, posElements.size(), GL_UNSIGNED_INT, 0);

    shader->release();
}

void mesh::remove() {
    qDebug()<<positions.size();
    VAO.destroy();
    VBO.destroy();
    EBO.destroy();
    shader->destroyed();
    delete shader;
    clearData();

    qDebug()<<"remove mesh: "<<QString::fromStdString(name);
}

void mesh::clearData()
{
    positions.clear();
    normals.clear();
    uvs.clear();
    posElements.clear();
    norElements.clear();
    uvElements.clear();
}

mesh::mesh()
    : VBO(QOpenGLBuffer::VertexBuffer)
    , EBO(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    //init data
    shader = new QOpenGLShaderProgram();
    QString path = "../OpenGL_Renderer/shader/";
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex,path + "mesh.vert");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment,path + "mesh.frag");
    shader->bind();
    shader->link();
    m_posAttr = shader->attributeLocation("posAttr");
    m_normAttr = shader->attributeLocation("normAttr");
    m_uvAttr = shader->attributeLocation("uvAttr");
    viewMatrix = shader->uniformLocation("viewMatrix");
    transMatrix = shader->uniformLocation("transMatrix");
    transform.setToIdentity();

    VAO.create();
    VBO.create();
    EBO.create();
}

mesh::~mesh() {
    remove();
}

void modle::draw(QMatrix4x4 matrix) {
    for(mesh* m:data)
    {
        m->draw(matrix);
    }
}

void modle::remove()
{
    for(mesh* m:data)
        delete m;

    data.clear();
    //qDebug()<<"remove Modle";

}

modle::~modle() {
    remove();
}
