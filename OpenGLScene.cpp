//
// Created by arno on 19-6-7.
//

#include "OpenGLScene.h"
#include <QMatrix4x4>
#include <QScreen>
#include <qmath.h>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QEvent>
#include <QtOpenGL>
#include <globalvar.h>

OpenGLScene::OpenGLScene(QWindow *parent) :OpenGLWindow(parent)
{
    hasInitialized = false;
}

OpenGLScene::~OpenGLScene()
{
    modles.clear();
    qDebug()<<"quit";
}

void OpenGLScene::initialize()
{
    //opengl
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_CULL_FACE);
    //glCullFace (GL_BACK);
    //glEnable(GL_STENCIL_TEST);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //scene modles
    std::string path = "/home/arno/Coding/Modles/d.obj";
    modles.push_back(modle::create(path));
    path = "/home/arno/Coding/Modles/untitled.obj";
    modles.push_back(modle::create(path));

    //camera
    cam = Camera::create(0,0,4);
    cam->updateProject(width(),height());

    //skybox
    skybox = Skybox::create();
    skydome = Skydome::create();
    std::string p = "../data/Skydome/Top_Of_West_Loop.hdr";
    skydome->setImage(p);
    hasInitialized = true;
}


void OpenGLScene::render()
{
    //glEnable(GL_DEPTH_TEST);
    cam->updateCamPos();
    QMatrix4x4 camMatrix = cam->getMatrix();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清除并渲染背景
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲和深度缓冲
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //skybox->draw(cam->getSkyMatrix());
    skydome->draw(cam->getSkyMatrix());
    for(modlePtr m:modles)
        m->draw(camMatrix);
}


void OpenGLScene::resizeEvent(QResizeEvent * event)
{
    if (!hasInitialized) return;

    int w = event->size().width();
    int h = event->size().height();
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, int(w * retinaScale), int(h * retinaScale));
    cam->updateProject(w,h);
    QWindow::resizeEvent(event);
}

void OpenGLScene::wheelEvent(QWheelEvent * event)
{
    //setRender(true);
    cam->MODE = cam->MODE_ZOOM;
    float delta = event->angleDelta().y();
    cam->speedZ = delta/5;
    //renderNow();
    QWindow::wheelEvent(event);
}

void OpenGLScene::mousePressEvent(QMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //cam->Target = cam->Position + cam->Forward*30.0f;
        cam->computeDistance();
        cam->MODE = cam->MODE_ROTATE;

    }
    else if (event->buttons() & Qt::MiddleButton)
    {
        cam->MODE = cam->MODE_MOVE;
    }
    else if (event->buttons() & Qt::RightButton)
    {
        cam->MODE = cam->MODE_ZOOM;
    }
    //setRender(true);
    QWindow::mousePressEvent(event);
}

void OpenGLScene::mouseMoveEvent(QMouseEvent * event)
{
    //setRender(true);

    float deltaX, deltaY;
    float posX = event->pos().x();
    float posY = event->pos().y();
    deltaX = posX - preX;
    deltaY = event->pos().y() - preY;
    preX = posX;
    preY = posY;

    if (event->buttons() & Qt::LeftButton)
    {
        cam->speedX = deltaX * 3;
        cam->speedY = deltaY * 3;
        cam->updateCamVectors();
        //qDebug() << "rotate";
    }
    else if (event->buttons() & Qt::MiddleButton)
    {

        cam->speedX = deltaX;
        cam->speedY = deltaY;
        //qDebug() << "move";
    }
    else if (event->buttons() & Qt::RightButton)
    {
        cam->speedZ = deltaX;
        //qDebug() << "zoom";
    }
    else
        return;

    //renderNow();

    QWindow::mouseMoveEvent(event);
}

void OpenGLScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F:
        cam->init(0,0,4);
        break;
    case Qt::Key_0:
        RENDER_MODE = GL_TRIANGLES;
        break;
    case Qt::Key_1:
        RENDER_MODE = GL_POINTS;
        break;
    case Qt::Key_2:
        RENDER_MODE = GL_LINES;
        break;
    case Qt::Key_3:
        RENDER_MODE = GL_LINE_LOOP;
        break;
    case Qt::Key_4:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case Qt::Key_5:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case Qt::Key_Down:
        for(modlePtr md:modles)
            for(meshPtr m:md->data)
                m->transform.translate(0,-0.1f,0);
        break;
    case Qt::Key_Up:
        for(modlePtr md:modles)
            for(meshPtr m:md->data)
                m->transform.translate(0,0.1f,0);
        break;
    default:
        RENDER_MODE = GL_TRIANGLES;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
//    if(event->key() == Qt::Key_F)
//    {
//        cam->init(0,0,4);
//    }

}


//void OpenGLScene::testDraw(QMatrix4x4 matrix)
//{
//    m_program->bind();
//    m_program->setUniformValue(m_matrixUniform, matrix);

//    GLuint vbo;
//    glGenBuffers(1,&vbo);
//    glBindBuffer(GL_ARRAY_BUFFER,vbo);
//    glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,GL_STATIC_DRAW);

//    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
//    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
//    //glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
//    // glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

//    glEnableVertexAttribArray(m_posAttr);
//    glEnableVertexAttribArray(m_colAttr);

//    glDrawArrays(GL_TRIANGLES, 0, 3);

//    glDisableVertexAttribArray(m_colAttr);
//    glDisableVertexAttribArray(m_posAttr);

//    m_program->release();

//}
