//
// Created by arno on 19-6-7.
//

#include "Model.h"
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
//#include <SOIL.h>

modelPtr model::create(std::string &objPath){
    modelPtr md{new model(objPath)};
    return md;
}

model::model(std::string &objPath) {
    path = objPath;
    load();
}

void model::setObj(std::string &objPath) {
    data.clear();
    path = objPath;
    load();
}

void model::load() {
    std::ifstream in(path.c_str(), std::ifstream::in);

    if (in.is_open()) {
        std::string line;
        meshPtr currentMesh = mesh::create();
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
                    currentMesh = mesh::create();
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
    for(meshPtr m:data)
        m->setup();
}

void mesh::setup(){
    if(posElements.size()==0) return;
    //std::vector<float> data;
    data.clear();
    for(int i=0;i<int(posElements.size())/3;++i)
    {
//        data.push_back(positions[posElements[i]*3]);
//        data.push_back(positions[posElements[i]*3+1]);
//        data.push_back(positions[posElements[i]*3+2]);

//        data.push_back(normals[norElements[i]*3]);
//        data.push_back(normals[norElements[i]*3+1]);
//        data.push_back(normals[norElements[i]*3+2]);

//        data.push_back(uvs[uvElements[i]*2]);
//        data.push_back(uvs[uvElements[i]*2+1]);

//        vertex v;
//        v.position.setX(positions[posElements[i]*3]);
//        v.position.setY(positions[posElements[i]*3+1]);
//        v.position.setZ(positions[posElements[i]*3+2]);

//        v.normal.setX(normals[norElements[i]*3]);
//        v.normal.setY(normals[norElements[i]*3+1]);
//        v.normal.setZ(normals[norElements[i]*3+2]);

//        v.uv.setX(uvs[uvElements[i]*2]);
//        v.uv.setY(uvs[uvElements[i]*2+1]);
//        data.push_back(v);
        triangle tr;
        for(int j=0;j<3;++j)
        {
            vertex v;
            v.position.setX(positions[posElements[i*3+j]*3]);
            v.position.setY(positions[posElements[i*3+j]*3+1]);
            v.position.setZ(positions[posElements[i*3+j]*3+2]);

            v.normal.setX(normals[norElements[i*3+j]*3]);
            v.normal.setY(normals[norElements[i*3+j]*3+1]);
            v.normal.setZ(normals[norElements[i*3+j]*3+2]);

            v.uv.setX(uvs[uvElements[i*3+j]*2]);
            v.uv.setY(uvs[uvElements[i*3+j]*2+1]);
            //tr.vertices.push_back(v);
            switch (j) {
            case 0:tr.pa = v;
                break;
            case 1:tr.pb = v;
                break;
            case 2:tr.pc = v;
                break;
            }
        }
        data.push_back(tr);
    }
    clearData();

    //bind buffer
    shader->bind();
    VAO.bind();
    VBO.bind();
    //EBO.bind();

    //VBO.allocate(&data,int(data.size())*sizeof(GLfloat));
    //VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    EBO.allocate(&posElements,int(posElements.size())*sizeof(GLfloat));

    //allocate VBO
    //glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(triangle), &data[0], GL_STATIC_DRAW);

    //data to shader
    //int stride = 8*sizeof(GLfloat);
    int stride = sizeof(vertex);
    shader->enableAttributeArray(shader->posAttr);
    //shader->setAttributeBuffer(shader->posAttr,GL_FLOAT,0,3,stride);
    shader->setAttributeBuffer(shader->posAttr,GL_FLOAT,0,3,stride);
    shader->enableAttributeArray(shader->normAttr);
    shader->setAttributeBuffer(shader->normAttr,GL_FLOAT,3*sizeof(GLfloat),3,stride);
    shader->enableAttributeArray(shader->uvAttr);
    shader->setAttributeBuffer(shader->uvAttr,GL_FLOAT,6*sizeof(GLfloat),2,stride);

    VAO.release();
    VBO.release();
    shader->release();
}

void mesh::draw(QMatrix4x4 projM,QMatrix4x4 viewM) {
    if(posElements.size()==0) return;

    //setup shader
    shader->bind();
    shader->setMatrix(projM,viewM,transform);
    // Draw mesh
    VAO.bind();
    glDrawArrays(RENDER_MODE, 0, int(posElements.size()));
    VAO.release();
    //EBO.bind()
    //glDrawElements(GL_TRIANGLES, posElements.size(), GL_UNSIGNED_INT, 0);

    shader->release();
}

void mesh::clearData(){
    positions.clear();
    normals.clear();
    uvs.clear();
    //posElements.clear();
    norElements.clear();
    uvElements.clear();
}

void mesh::setShader(shaderPtr s){
    shader = s;
}

meshPtr mesh::create()
{
    meshPtr ms{new mesh()};
    return ms;
}

mesh::mesh()
    : VBO(QOpenGLBuffer::VertexBuffer)
    , EBO(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    //init gl data
    std::string path = "../OpenGL_Renderer/shader/";
    std::string vert = path + "mesh.vert";
    std::string frag = path + "mesh.frag";

    shader = Shader::Create(vert,frag);
    transform.setToIdentity();

    VAO.create();
    VBO.create();
    EBO.create();
}

mesh::~mesh() {
    qDebug()<<positions.size();
    VAO.destroy();
    VBO.destroy();
    EBO.destroy();
    shader->destroyed();
    clearData();
    posElements.clear();
    qDebug()<<"remove mesh: "<<QString::fromStdString(name);
}


void model::draw(QMatrix4x4 projM,QMatrix4x4 viewM) {
    for(meshPtr m:data)
        m->draw(projM,viewM);
}

void model::setShader(shaderPtr s){
    for(meshPtr m:data)
        m->setShader(s);
}


model::~model() {
    data.clear();
}
