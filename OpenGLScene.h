//
// Created by arno on 19-6-7.
//

#pragma once

#include <oglWindow.h>
#include <Model.h>
#include <memory>
#include <QMatrix4x4>
#include <Camera.h>
#include <QMouseEvent>
#include <Skybox.h>

class OpenGLScene :public OpenGLWindow
{
public:

    OpenGLScene(QWindow *parent = nullptr);
    ~OpenGLScene() override;

    void initialize() override;
    void render() override ;

private:
    CameraPtr cam;
    SkyboxPtr skybox;
    SkydomePtr skydome;
    std::vector<modelPtr> models;

    bool hasInitialized;
    float preX;
    float preY;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};


