//
// Created by arno on 19-6-7.
//

#pragma once

#include <oglWindow.h>
#include <Modle.h>
#include <memory>
#include <QMatrix4x4>
#include <Camera.h>
#include <QMouseEvent>

class OpenGLScene :public OpenGLWindow
{
public:

    OpenGLScene(QWindow *parent = nullptr);
    ~OpenGLScene() override;

    void initialize() override;
    void render() override ;

private:
    Camera *cam;
    std::vector<modle*> modles;

    bool hasInitialized;
    float preX;
    float preY;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};


